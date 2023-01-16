#include "minishell.h"

int	redirect_output(char *str, int mode)
{
	int		i;
	int		j;
	char	*path;

	i = 0;
	j = 0;
	path = malloc(ft_strlen(str) * sizeof(char *));
	while (str[i] != '>' && str[i] != '\0')
		i++;
	i++;
	if (str[i] == '>')
		i ++;
	while (str[i] != '\0' && (str[i] == ' ' || str[i] == '\t'))
		i ++;
	while (str[i] != '\0' && str[i] != ' ' && str[i] != '\t' && str[i] != '|')
		path[j++] = str[i++];
	path[j] = '\0';
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
		path[j++] = str[i++];
	path[j] = '\0';
	fd = open(path, O_RDONLY);
	dup2(fd, 0);
	close(fd);
	free(path);
}