/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amillahadzic <amillahadzic@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 20:27:14 by amillahadzi       #+#    #+#             */
/*   Updated: 2023/01/23 14:54:15 by ykimirti         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

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

// "   \t\t\t "
int	ft_command_count(char *str)
{
	int		i;
	int		size;
	char	current_quote;

	if (str == NULL)
		return (0);
	current_quote = '\0';
	i = 0;
	size = 1;
	skip_spaces(str, &i);
	if (str[i] == '\0')
		return (0);
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

void	print_list(char **list)
{
	int	i;

	i = 0;
	while (list[i] != NULL)
		printf("> %s\n", list[i++]);
}
