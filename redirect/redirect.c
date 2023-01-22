#include "../minishell.h"

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
	free(path);
	if (file == -1)
	{
		perror("error opening file");
		g_x->redirect_error = 1;
		return (-1);
	}
	return (file);
}