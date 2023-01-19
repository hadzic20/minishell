#include "minishell.h"

// Skips from one quote to end of the other
void	skip_quote(char *s, int *i)
{
	char	quote;

	quote = *s;
	(*i)++;
	while (*s != quote)
	{
		if (*s == '\0')
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

	printf("> RAW: >%s<\n", cmd);
	cmd = g_x->cmds[cmd_index].raw_command;
	i = 0;
	while (cmd[i] != '\0' && g_x->error_code == 0)
	{
		if (cmd[i] == '\"' || cmd[i] == '\'')
			skip_quote(cmd, &i);
		if (cmd[i] == '\0')
			return ;
		if (cmd[i] == '>' && cmd[i + 1] == '>')
			g_x->cmds[cmd_index].outfile = redirect_output(cmd, &i, true);
		else if (cmd[i] == '>')
			g_x->cmds[cmd_index].outfile = redirect_output(cmd, &i, false);
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
		file = open(path, O_CREAT | O_RDWR | O_TRUNC, 0777);
	else
		file = open(path, O_CREAT | O_RDWR | O_APPEND, 0777);
	if (file == -1)
	{
		perror("error opening file");
		g_x->error_code = 1;
		return (-1);
	}
	return (file);
}

int	redirect_output_old(char *str, int mode)
{
	int		i;
	int		j;
	int		temp;
	char	*path;
	char	current_quote;

	current_quote = '\0';
	i = 0;
	j = 0;
	path = malloc(ft_strlen(str) * sizeof(char *));
	while (str[i] != '>' && str[i] != '\0')
	{
		if (current_quote == '\0' && (str[i] == '"' || str[i] == '\''))
			current_quote = str[i];
		else if (current_quote != '\0' && str[i] == current_quote)
			current_quote = '\0';
		i++;
	}
	if (current_quote != '\0')
		return (1);
	i++;
	if (str[i] == '>')
		i++;
	path[j] = '\0';
	printf("output file is %s\n", path);
	if (mode == 0)
		j = open(path, O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (mode == 1)
		j = open(path, O_CREAT | O_RDWR | O_APPEND, 0777);
	free (path);
	return (j);
}

void	redirect_input(char *str)
{
	int		fd;
	int		i;
	int		j;
	int		temp;
	char	*path;

	i = 0;
	j = 0;
	path = malloc(ft_strlen(str) * sizeof(char *));
	while (str[i] != '<' && str[i] != '\0')
		i++;
	i++;
	while (str[i] != '\0' && (str[i] == ' ' || str[i] == '\t'))
		i ++;
	while (str[i] != '\0' && str[i] != ' ' && str[i] != '\t' && str[i] != '|')
	{
		temp = i;
		if (str[i] == '"')
			path = ft_strjoin(path, double_quote(str, &i));
		else if (str[i] == '\'')
			path = ft_strjoin(path, quote(str, &i));
		j = j + i - temp;
		path[j++] = str[i++];
	}
	path[j] = '\0';
	fd = open(path, O_RDONLY);
	dup2(fd, 0);
	close(fd);
	free(path);
}

void	heredoc(char *str)
{
	int		i;
	int		j;
	int		temp;
	char	*path;
	char	*line;
	char	*bucket;
	char	current_quote;

	current_quote = '\0';
	i = 0;
	j = 0;
	path = malloc(ft_strlen(str) * sizeof(char *));
	while (str[i] != '<' && str[i] != '\0')
	{
		if (current_quote == '\0' && (str[i] == '"' || str[i] == '\''))
			current_quote = str[i];
		else if (current_quote != '\0' && str[i] == current_quote)
			current_quote = '\0';
		i++;
	}
	if (current_quote != '\0')
		return ;
	i += 2;
	while (str[i] != '\0' && (str[i] == ' ' || str[i] == '\t'))
		i ++;
	while (str[i] != '\0' && str[i] != ' ' && str[i] != '\t' && str[i] != '|')
	{
		temp = i;
		if (str[i] == '"')
			path = ft_strjoin(path, double_quote(str, &i));
		else if (str[i] == '\'')
			path = ft_strjoin(path, quote(str, &i));
		j = j + i - temp;
		path[j++] = str[i++];
	}
	path[j] = '\0';
	printf("heredoc is %s\n", path);
	line = readline("heredoc> ");
	bucket = malloc(sizeof(char));
	while (!(ft_strnstr(line, path, ft_strlen(path)) && line[ft_strlen(path)] == '\0'))
	{
		line = ft_strjoin(line, "\n");
		bucket = ft_strjoin(bucket, line);
		line = readline("heredoc> ");
	}
	printf("%s", bucket);
}
