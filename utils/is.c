/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amillahadzic <amillahadzic@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 20:27:10 by amillahadzi       #+#    #+#             */
/*   Updated: 2023/01/23 13:59:39 by ykimirti         ###   ########.tr       */
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

bool	is_seperator(char c)
{
	return (ft_isspace(c)
		|| c == '<'
		|| c == '>'
		|| c == '\0'
		|| c == '|');
}
