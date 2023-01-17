#include "../minishell.h"

int	ft_word_count(char *str)
{
	int	i;
	int	size;

	i = 0;
	size = 0;
	while (str[i] != '\0')
	{
		if (str[i] != ' ' && str[i] != '\t' && \
		(str[i + 1] == ' ' || str[i + 1] == '\0'))
			size++;
		i++;
	}
	return (size);
}

int	ft_command_count(char *str)
{
	int	i;
	int	size;

	i = 0;
	size = 1;
	while (str[i] != '\0')
	{
		if (str[i] == '|' && str[i + 1] != '|')
			size++;
		i++;
	}
	return (size);
}

int	ft_str2len(char **str2)
{
	int	i;

	i = 0;
	while (str2[i])
		i++;
	return (i);
}