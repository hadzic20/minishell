#include "minishell.h"

bool	is_metachar(char c)
{
	return (c == '"'
		|| c == '\''
		|| c == '>'
		|| c == '<'
		|| c == '|'
		|| c == '$');
}

bool	ft_isspace(char c)
{
	return (c == ' ' || c == '\t');
}

void	skip_spaces(char *s, int *i)
{
	while (ft_isspace(s[*i]))
		(*i)++;
}

// Redirection skipleme
void	skip_redirection(char *s, int *i)
{
	(*i)++;
	skip_spaces(s, i);
	while (s[*i] != '\0' && !ft_isspace(s[*i]) && s[*i] != '|')
		(*i)++;
	skip_spaces(s, i);
}

/* Returns false when sees a redirection so it can be skipped
 *  without increasing j
 */
// TODO: Memory leakler
bool	expand_single(char *s, int *i, char **dst)
{
	int		k;

	if (s[*i] == '"')
		*dst = strjoin_free(*dst, double_quote(s, i)); // leak yok???
	else if (s[*i] == '\'')
		*dst = strjoin_free(*dst, quote(s, i)); // bunda da leak yok. dolarda neden var?
	else if (s[*i] == '$')
		*dst = strjoin_free(*dst, dollar(s, i)); // leak ama neden?
	else if (s[*i] == '<' || s[*i] == '>')
	{
		skip_redirection(s, i);
		return (false);
	}
	else
	{
		k = ft_strlen(*dst);
		*dst = strjoin_free(*dst, ft_strdup(s));
		while (s[*i] != '\0' && !is_metachar(s[*i]) && !ft_isspace(s[*i]))
			(*dst)[k++] = s[(*i)++];
		(*dst)[k] = '\0';
	}
	return (true);
}

void	print_list(char **list)
{
	int	i;

	i = 0;
	while (list[i] != NULL)
		printf("> %s\n", list[i++]);
}

// Burada leakler biraz sıkıntılı
char	**extract_command(char *s)
{
	int		i;
	int 	j;
	char	**command;
	int		len;

	j = 0;
	if (ft_word_count(s) == 0)
		return (NULL);
	command = malloc(1 * sizeof(char *));
	command[0] = NULL;
	i = 0;
	len = 0;
	while (s[i] != '\0')
	{
		skip_spaces(s, &i);
		if (s[i] == '\0')
			break ;
		if (len <= j)
		{
			printf("j is %d\n", j);
			command = ft_rrealloc(command, j + 1);
			command[j] = malloc(ft_strlen(s) * sizeof(char));
			command[j][0] = '\0';
			command[j + 1] = NULL;
			len = j;
		}
		if (!expand_single(s, &i, &command[j]))
			continue ;
		if (ft_isspace(s[i]) || s[i] == '\0')
			j++;
	}
	if (command[j])
		free(command[j]);
	command[j] = NULL;
	return (command);
}

// OK
void	seperate_command(char *s)
{
	int	i;
	int	j;
	int	k;
	char	current_quote;

	current_quote = '\0';
	i = -1;
	j = 0;
	g_x->cmds = malloc((g_x->cmd_count)*sizeof(t_command));
	while (++i < g_x->cmd_count)
		g_x->cmds[i].raw_command = ft_calloc((ft_strlen(s) + 1), sizeof(char)); // malloc -> calloc
	i = 0;
	while (s[i] != '\0')
	{
		k = 0;
		while (s[i] != '\0' && (s[i] != '|' || current_quote != '\0'))
		{
			if (current_quote == '\0' && (s[i] == '"' || s[i] == '\''))
				current_quote = s[i];
			else if (current_quote != '\0' && s[i] == current_quote)
				current_quote = '\0';
			g_x->cmds[j].raw_command[k++] = s[i++];
		}
		g_x->cmds[j].raw_command[k] = '\0';
		if (s[i] == '|')
			i++;
		j++;
	}
}

// "asd"
char	*double_quote(char *s, int *i)
{
	int		j;
	char	*command;
	char	*temp;

	j = 0;
	command = (char *)malloc((ft_strlen(s) + 1) * sizeof(char));
	command[0] = '\0';
	(*i)++;
	while (s[*i] != '"')
	{
		if (s[*i] == '$')
		{
			temp = dollar(s, i);
			command = ft_strjoin(command, temp);
			j = ft_strlen(command);
		}
		else if (s[*i] == '\0')
		{
			print_error("minishell", "command", "unclosed double quote");
			return (NULL);
		}
		else
			command[j++] = s[(*i)++];
	}
	command[j] = '\0';
	if (s[*i] != '\0')
		(*i)++;
	return (command);
}

// MUK
char	*quote(char *s, int *i)
{
	int		j;
	char	*command;

	j = 0;
	command = (char *)malloc(ft_strlen(s) * sizeof(char));
	(*i)++;
	while (s[*i] != '\'')
	{
		if (s[*i] == '\0')
		{
			print_error("minishell", "command", "unclosed quote");
			return (NULL);
		}
		command[j++] = s[(*i)++];
	}
	command[j] = '\0';
	if (s[*i] != '\0')
		(*i)++;
	return (command);
}

void	ft_exit(char **command)
{
	int	status;

	status = 0;
	if (command[0] != NULL && command[1] != NULL)
		status = ft_atoi(command[1]);
	exit(status);
}

// Eger bir forkun icindeyse execve yapmadan once kendisi forklamasina
// gerek yok
void handle_command(char **command, int outfd, int infd, bool is_in_fork)
{
	int	pid;
	int	status;

	if (command == NULL)
		return ;
	if (ft_strncmp(command[0], "cd", sizeof("cd")) == 0)
		ft_change_dir(command[1]);
	else if (ft_strncmp(command[0], "pwd", sizeof("pwd")) == 0)
		mini_pwd(outfd);
	else if (ft_strncmp(command[0], "env", sizeof("env")) == 0)
		mini_env(outfd);
	else if (ft_strncmp(command[0], "echo", sizeof("echo")) == 0)
		mini_echo(command, outfd);
	else if (ft_strncmp(command[0], "export", sizeof("export")) == 0)
		ft_export(command, outfd);
	else if (ft_strncmp(command[0], "unset", sizeof("unset")) == 0)
		ft_unset(command);
	else if (ft_strncmp(command[0], "exit", sizeof("exit")) == 0)
		ft_exit(command);
	else if (!is_in_fork)
	{
		pid = fork();
		if (pid == 0)
		{
			mini_pathed(command, outfd, infd);
		}
		status = 0;
		waitpid(pid, &status, 0);
		g_x->error_code = WEXITSTATUS(status);
	}
	else
		mini_pathed(command, outfd, infd);
}

// Give which command to execute and execute that.
void handle_command_execution(int i, bool is_in_fork)
{
	g_x->cmds[i].handled_cmd = extract_command(g_x->cmds[i].raw_command);
	g_x->error_code = 0;

	handle_command(g_x->cmds[i].handled_cmd, g_x->cmds[i].outfile,
		g_x->cmds[i].infile, is_in_fork);
}

void handle_line_utils(int i, int save_fd, char *str)
{
	if (i != 0)
	{
		dup2(save_fd, 0);
		close(save_fd);
	}
	close(g_x->cmds[i].p[0]);
	if (i != g_x->cmd_count - 1)
		dup2(g_x->cmds[i].p[1], 1);
	close(g_x->cmds[i].p[1]);
	handle_command_execution(i, true);
}

void handle_line(char *str)
{
	int i;
	int save_fd;
	int last_pid;
	int status;
	int pid;

	if (g_x->cmd_count == 0)
		return ;
	seperate_command(str);
	status = 0;
	if (g_x->cmd_count == 1)
	{
		redirect(0);
		if (g_x->redirect_error == 0)
			handle_command_execution(0, false);
		else
			g_x->error_code = g_x->redirect_error;
	}
	else
	{
		i = -1;
		while (++i != g_x->cmd_count && g_x->cmd_count != 1)
		{
			redirect(i);
			if (g_x->redirect_error != 0)
			{
				g_x->error_code = g_x->redirect_error;
				return ;
			}
			g_x->cmds[i].outfile = 1;
			pipe(g_x->cmds[i].p);
			pid = fork();
			last_pid = pid;
			if (pid == 0)
			{
				handle_line_utils(i, save_fd, str);
				// Eğer bir builtinse ve hata yoksa buraya kod gelebilir...
				exit(g_x->error_code);
			}
			close(g_x->cmds[i].p[1]);
			if (i != 0)
				close(save_fd);
			save_fd = g_x->cmds[i].p[0];
		}
		close(save_fd);
		status = 0;
		waitpid(last_pid, &status, 0);
		g_x->error_code = WEXITSTATUS(status);
	}
	while (waitpid(-1, &status, 0) != -1)
		;
}

void	handle_signal(int signum)
{
	if (signum == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		//rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handle_crtl_d(void)
{
	printf("exit\n");
	exit(0);
}
