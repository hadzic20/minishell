/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amillahadzic <amillahadzic@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 20:20:08 by amillahadzi       #+#    #+#             */
/*   Updated: 2023/01/22 20:20:09 by amillahadzi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*double_quote(char *s, int *i)
{
	int		j;
	char	*command;
	char	*temp;

	j = 0;
	command = (char *)malloc((ft_strlen(s) + 1) * sizeof(char));
	command[0] = '\0';
	(*i)++;
	while (s[*i] != '"')
	{
		if (s[*i] == '$')
		{
			temp = dollar(s, i);
			command = ft_strjoin(command, temp);
			j = ft_strlen(command);
		}
		else if (s[*i] == '\0')
		{
			print_error("minishell", "command", "unclosed double quote");
			return (NULL);
		}
		else
			command[j++] = s[(*i)++];
	}
	command[j] = '\0';
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
	while (s[*i] != '\'')
	{
		if (s[*i] == '\0')
		{
			print_error("minishell", "command", "unclosed quote");
			return (NULL);
		}
		command[j++] = s[(*i)++];
	}
	command[j] = '\0';
	if (s[*i] != '\0')
		(*i)++;
	return (command);
}
