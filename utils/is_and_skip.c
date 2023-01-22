/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_and_skip.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amillahadzic <amillahadzic@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 20:27:10 by amillahadzi       #+#    #+#             */
/*   Updated: 2023/01/22 20:27:26 by amillahadzi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_metachar(char c)
{
	return (c == '"'
		|| c == '\''
		|| c == '>'
		|| c == '<'
		|| c == '|'
		|| c == '$');
}

bool	ft_isspace(char c)
{
	return (c == ' ' || c == '\t');
}

void	skip_spaces(char *s, int *i)
{
	while (ft_isspace(s[*i]))
		(*i)++;
}

void	skip_redirection(char *s, int *i)
{
	(*i)++;
	if (s[*i] == '<' || s[*i] == '>')
		(*i)++;
	skip_spaces(s, i);
	while (s[*i] != '\0' && !ft_isspace(s[*i]) && s[*i] != '|')
		(*i)++;
	skip_spaces(s, i);
}

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
