/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amillahadzic <amillahadzic@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 20:21:48 by amillahadzi       #+#    #+#             */
/*   Updated: 2023/01/23 14:19:19 by ykimirti         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*status_case(char *s, int *i)
{
	char	*str;

	(void)s;
	str = ft_itoa(g_x->error_code);
	(*i)++;
	return (str);
}

char	*handle_env_substr(const char *env_var, char *command)
{
	char	*new;

	new = ft_substr(env_var, ft_strlen(command), ft_strlen(env_var));
	free(command);
	return (new);
}

// MUK
char	*dollar(char *s, int *i)
{
	int		j;
	char	*command;

	j = 0;
	command = (char *)malloc((ft_strlen(s) + 10) * sizeof(char));
	(*i)++;
	if (s[*i] == '?')
		return (free(command), status_case(s, i));
	if (!ft_isalnum(s[*i]) && s[*i] != '_')
		return (free(command), ft_strdup("$"));
	while (ft_isalnum(s[*i]) || s[*i] == '_')
		command[j++] = s[(*i)++];
	command[j] = '=';
	command[j + 1] = '\0';
	j = -1;
	while (++j < ft_str2len(g_x->export))
		if (ft_strnstr(g_x->export[j], command, ft_strlen(command)) != 0)
			return (handle_env_substr(g_x->export[j], command));
	return (free(command), ft_strdup(""));
}
