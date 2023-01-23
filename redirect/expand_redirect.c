/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykimirti <ykimirti@42istanbul.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 16:53:09 by ykimirti          #+#    #+#             */
/*   Updated: 2023/01/23 16:53:11 by ykimirti         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Same as expand_arg but first skips spaces and
// prints an error if it returns null.
char	*expand_redirect(char *s, int *i)
{
	char	*file;

	skip_spaces(s, i);
	file = expand_arg(s, i);
	if (!file)
	{
		g_x->redirect_error = 2;
		return (print_error("minishell", "redirection",
				"expected filename after redirection"), NULL);
	}
	return (file);
}
