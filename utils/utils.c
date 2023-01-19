#include "../minishell.h"

int	ft_word_count(char *str)
{
	int	i;
	int	size;

	i = 0;
	size = 0;
	if (str == NULL)
		return (0);
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
	char	current_quote;

	current_quote = '\0';
	i = 0;
	size = 1;
	while (str[i] != '\0')
	{
		if (current_quote == '\0' && (str[i] == '"' || str[i] == '\''))
			current_quote = str[i];
		else if (current_quote != '\0' && str[i] == current_quote)
			current_quote = '\0';
		if (str[i] == '|' && current_quote == '\0')
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
