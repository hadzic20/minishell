#include "minishell.h"

bool	is_metachar(char c)
{
	return (c == '"' ||
			c == '\'' ||
			c == '>' ||
			c == '<' ||
			c == '|' ||
			c == '$');
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

void	skip_redirection(char *s, int *i)
{
	// Redirection skipleme
	(*i)++;
	skip_spaces(s, i);
	while (s[*i] != '\0' && !ft_isspace(s[*i]) && s[*i] != '|')
		(*i)++;
	skip_spaces(s, i);
}

/* Returns false when sees a redirection so it can be skipped
 *  without increasing j
 */
bool	expand_single(char *s, int *i, char **dst)
{
	int	k;

	// command uninitialized haliyle strjoin yapiliyor
	if (s[*i] == '"')
	{
		//printf("before join is >%s<\n", *dst);
		*dst = ft_strjoin(*dst, double_quote(s, i));
		//printf("after join is >%s<\n", *dst);
	}
	else if (s[*i] == '\'')
		*dst = ft_strjoin(*dst, quote(s, i));
	else if (s[*i] == '$')
{
		//printf("> before join is >%s<\n", *dst);
		*dst = ft_strjoin(*dst, dollar(s, i));
		//printf("> after join is >%s<\n", *dst);
}
		// Sunu isspace fonksyonu olarak ayir
	else if (s[*i] == '<' || s[*i] == '>')
	{
		// Redirection skiple
		skip_redirection(s, i);
		return (false);
	}
	// Argumani expand et
	else
	{
		k = 0;
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

char **extract_command(char *s)
{
	int i;
	int j;
	char **command;

	j = 0;
	command = (char **)malloc((ft_word_count(s) + 1) * sizeof(char *));
	i = -1;
	while (++i <= ft_word_count(s))
	{	
		command[i] = (char *)malloc(ft_strlen(s) * sizeof(char));
		command[i][0] = '\0';
	}
	i = 0;
	while (s[i] != '\0')
	{
		skip_spaces(s, &i);
		if (s[i] == '\0')
			break ;
		if (!expand_single(s, &i, &command[j]))
			continue ;
		if (ft_isspace(s[i]) || s[i] == '\0')
			j++;
	}
	command[j] = NULL;
	return (command);
}

// 
void	seperate_command(char *s)
{
	int	i;
	int	j;
	int	k;
	char	current_quote;

	current_quote = '\0';
	i = -1;
	j = 0;
	g_x->cmds = (t_command *)malloc((ft_command_count(s) + 1) * \
	sizeof(t_command));
	while (++i <= ft_command_count(s))
		g_x->cmds[i].raw_command = (char *)malloc(ft_strlen(s) * sizeof(char));
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
	g_x->cmds[j].raw_command = NULL;
}

char	*double_quote(char *s, int *i)
{
	int		j;
	char	*command;
	char	*temp;

	j = 0;
	command = (char *)malloc(ft_strlen(s) * sizeof(char));
	(*i)++;
	while (s[*i] != '"')
	{
		if (s[*i] == '$')
		{
			temp = dollar(s, i);
			command = ft_strjoin(command, temp);
			j = ft_strlen(command) - 1;
		}
		else if (s[*i] == '\0')
		{
			print_error("minishell", "command", "unclosed double quote");
			return (NULL);
		}
		else
			command[j++] = s[(*i)++];
	}
	if (s[*i] != '\0')
		(*i)++;
	return (command);
}

char	*quote(char *s, int *i)
{
	int		j;
	char	*command;

	j = 0;
	command = (char *)malloc(ft_strlen(s) * sizeof(char));
	(*i)++;
	while (s[*i] != 39)
	{
		if (s[*i] == '\0')
		{
			print_error("minishell", "command", "unclosed quote");
			return (NULL);
		}
		if (s[*i] == '"')
			command = ft_strjoin(command, double_quote(s, i));
		command[j++] = s[(*i)++];
	}
	if (s[*i] != '\0')
		(*i)++;
	return (command);
}

void ft_exit(char **command)
{
	int	status;

	status = 0;
	if (command[0] != NULL && command[1] != NULL)
		status = ft_atoi(command[1]);
	exit(status);
}

// Eger bir forkun icindeyse execve yapmadan once kendisi forklamasina
// gerek yok
void handle_command(char **command, int fd, bool is_in_fork)
{
	g_x->error_code = 0;
	if (ft_strnstr(command[0], "cd", 2) && command[0][2] == '\0')
		ft_change_dir(command[1]);
	else if (ft_strnstr(command[0], "pwd", 3) && command[0][3] == '\0')
		mini_pwd(fd);
	else if (ft_strnstr(command[0], "env", 3) && command[0][3] == '\0')
		mini_env(fd);
	else if (ft_strnstr(command[0], "echo", 4) && command[0][4] == '\0')
		mini_echo(command, fd);
	else if (ft_strnstr(command[0], "export", 6) && command[0][6] == '\0')
		ft_export(command, fd);
	else if (ft_strnstr(command[0], "unset", 5) && command[0][5] == '\0')
		ft_unset(command);
	else if (ft_strnstr(command[0], "exit", 4) && command[0][4] == '\0')
		ft_exit(command);
	else
	{
		if (!is_in_fork)
		{
			if (fork() != 0)
				return ;
		}
		mini_pathed(command, fd);
	}
	free(command[0]);
}

// Give which command to execute and execute that.
void handle_command_execution(int i, bool is_in_fork)
{
	g_x->cmds[i].handled_cmd = extract_command(g_x->cmds[i].raw_command);
	handle_command(g_x->cmds[i].handled_cmd, g_x->cmds[i].outfile, is_in_fork);
}

void handle_line_utils(int i, int save_fd, char *str)
{
	if (i != 0)
	{
		dup2(save_fd, 0);
		close(save_fd);
	}
	close(g_x->cmds[i].p[0]);
	if (i != ft_command_count(str) - 1)
		dup2(g_x->cmds[i].p[1], 1);
	close(g_x->cmds[i].p[1]);
	handle_command_execution(i, false);
}

void handle_line(char *str)
{
	int i;
	int save_fd;
	int last_pid;
	int pid;

	i = -1;
	seperate_command(str);
	if (ft_command_count(str) == 1)
	{
		handle_command_execution(0, true);
		return ;
	}
	while (g_x->cmds[++i].raw_command != NULL && ft_command_count(str) != 1)
	{
		g_x->cmds[i].outfile = 1;
        pipe(g_x->cmds[i].p);
		pid = fork();
		last_pid = pid;
		if (pid == 0)
		{
			redirect(i);
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
	
	int status;
	status = 0;
	
	waitpid(last_pid, &status, 0);
	// Set ? to WEXITSTATUS(status)
	g_x->error_code = WEXITSTATUS(status);
	while (waitpid(-1, &status, 0) != -1)
		;
	add_history(str);
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
	exit(1);
}
