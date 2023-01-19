#include "minishell.h"

// Skips from one quote to end of the other
void	skip_quote(char *s, int *i)
{
	char	quote;

	quote = s[*i];
	(*i)++;
	while (s[*i] != quote)
	{
		if (s[*i] == '\0')
			return ;
		(*i)++;
	}
	(*i)++;
}

// Returns the outfile
void	redirect(int cmd_index)
{
	char	*cmd;
	int		i;

	cmd = g_x->cmds[cmd_index].raw_command;
	g_x->cmds[cmd_index].infile = 0;
	g_x->cmds[cmd_index].outfile = 0;
	i = 0;
	g_x->redirect_error = 0;
	while (cmd[i] != '\0' && g_x->redirect_error == 0)
	{
		if (cmd[i] == '\"' || cmd[i] == '\'')
			skip_quote(cmd, &i);
		if (cmd[i] == '\0')
			return ;
		if (cmd[i] == '>' && cmd[i + 1] == '>')
			g_x->cmds[cmd_index].outfile = redirect_output(cmd, &i, true);
		else if (cmd[i] == '>')
			g_x->cmds[cmd_index].outfile = redirect_output(cmd, &i, false);
		else if (cmd[i] == '<' && cmd[i + 1] == '<')
			g_x->cmds[cmd_index].infile = heredoc(cmd, &i);
		else if (cmd[i] == '<')
			g_x->cmds[cmd_index].infile = redirect_input(cmd, &i);
		if (cmd[i] != '\0')
			i++;
	}
}

// ls >ahmet\t
char	*redirect_path(char *s, int *i)
{
	char	*final;

	final = ft_strdup("");
	while (s[*i] != '\0')
	{
		skip_spaces(s, i);
		if (s[*i] == '\0')
			return (NULL);
		if (!expand_single(s, i, &final))
			continue ;
		if (ft_isspace(s[*i]) || s[*i] == '\0')
			break ;
	}
	return (final);
}

int	redirect_input(char *str, int *i)
{
	char	*path;
	int		file;

	(*i) += 1;
	path = redirect_path(str, i);
	file = open(path, O_RDONLY, 0777);
	if (file == -1)
	{
		perror("error opening file");
		g_x->redirect_error = 1;
		return (-1);
	}
	return (file);
}

int	redirect_output(char *str, int *i, bool is_append)
{
	char	*path;
	int		file;

	if (is_append)
		(*i) += 2;
	else
		(*i) += 1;
	path = redirect_path(str, i);
	if (is_append)
		file = open(path, O_CREAT | O_RDWR | O_APPEND, 0777);
	else
		file = open(path, O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (file == -1)
	{
		perror("error opening file");
		g_x->redirect_error = 1;
		return (-1);
	}
	return (file);
}

// C-c to heredoc
void	heredoc_handler(int sig)
{
	(void)sig;
	g_x->redirect_error = 130;
	close(0);
}

void	heredoc_loop(const char *file, int fd)
{
	char	*line;

	while (true)
	{
		line = readline("> ");
		if (line == NULL)
			break ;
		if (ft_strncmp(line, file, ft_strlen(file) + 1) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
}

int	heredoc(char *str, int *i)
{
	int		fds[2];
	void	(*mem_handler)(int);
	int		readline_dup;
	char	*file;

	(*i) += 2;
	file = redirect_path(str, i);
	readline_dup = dup(0);
	if (pipe(fds) == -1)
		return (-1);
	mem_handler = signal(SIGINT, heredoc_handler);
	heredoc_loop(file, fds[1]);
	signal(SIGINT, mem_handler);
	dup2(readline_dup, 0);
	close(readline_dup);
	close(fds[1]);
	if (g_x->redirect_error != 0)
	{
		close(fds[0]);
		return (-1);
	}
	return (fds[0]);
}
