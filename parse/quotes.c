/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amillahadzic <amillahadzic@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 20:20:08 by amillahadzi       #+#    #+#             */
/*   Updated: 2023/01/23 14:42:51 by ykimirti         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*double_quote_helper(char *s, int *i, char *command)
{
	int	j;

	j = 0;
	while (s[*i] != '"')
	{
		if (s[*i] == '\0')
			return (free(command), print_error("minishell", "command",
					"unclosed double quote"), NULL);
		if (s[*i] == '$')
		{
			command[j] = '\0';
			command = strjoin_free(command, dollar(s, i));
			j = ft_strlen(command);
			command = strjoin_free(command, ft_strdup(s));
			command[j] = '\0';
		}
		else
			command[j++] = s[(*i)++];
	}
	command[j] = '\0';
	return (command);
}

char	*double_quote(char *s, int *i)
{
	char	*command;

	command = (char *)malloc((ft_strlen(s) + 1) * sizeof(char));
	command[0] = '\0';
	(*i)++;
	command = double_quote_helper(s, i, command);
	if (s[*i] == '"')
		(*i)++;
	return (command);
}

char	*quote(char *s, int *i)
{
	int		j;
	char	*command;

	j = 0;
	command = (char *)malloc((ft_strlen(s) + 1) * sizeof(char));
	(*i)++;
	while (s[*i] != '\'')
	{
		if (s[*i] == '\0')
		{
			print_error("minishell", "command", "unclosed quote");
			return (free(command), NULL);
		}
		command[j++] = s[(*i)++];
	}
	command[j] = '\0';
	if (s[*i] != '\0')
		(*i)++;
	return (command);
}

void	the_quotes(char *current_quote, char *s, int i)
{
	if (*current_quote == '\0' && (s[i] == '"' || s[i] == '\''))
		*current_quote = s[i];
	else if (*current_quote != '\0' && s[i] == *current_quote)
		*current_quote = '\0';
}
