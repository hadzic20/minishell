#include "minishell.h"

void	redirect(int i)
{
	if (ft_strnstr(g_x->cmds[i].raw_command, ">>", ft_strlen(g_x->cmds[i].raw_command)))
		g_x->cmds[i].outfile = redirect_output(g_x->cmds[i].raw_command, 1);
    else if (ft_strchr(g_x->cmds[i].raw_command, '>'))
    	g_x->cmds[i].outfile = redirect_output(g_x->cmds[i].raw_command, 0);
	if (ft_strnstr(g_x->cmds[i].raw_command, "<<", ft_strlen(g_x->cmds[i].raw_command)))
		heredoc(g_x->cmds[i].raw_command);
	else if (ft_strchr(g_x->cmds[i].raw_command, '<'))
    	redirect_input(g_x->cmds[i].raw_command);
}

int	redirect_output(char *str, int mode)
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
		i ++;
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