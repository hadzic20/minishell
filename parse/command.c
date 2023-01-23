/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amillahadzic <amillahadzic@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 20:23:04 by amillahadzi       #+#    #+#             */
/*   Updated: 2023/01/23 16:28:00 by ykimirti         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Expand quote, dquote, dollar or word UNTIL it sees a
// ft_isspace, 
void	expand_single(char *s, int *i, char **dst)
{
	int		k;

	if (s[*i] == '"')
		*dst = strjoin_free(*dst, double_quote(s, i));
	else if (s[*i] == '\'')
		*dst = strjoin_free(*dst, quote(s, i));
	else if (s[*i] == '$')
		*dst = strjoin_free(*dst, dollar(s, i));
	else
	{
		k = ft_strlen(*dst);
		*dst = strjoin_free(*dst, ft_strdup(s));
		while (s[*i] != '\0' && !is_metachar(s[*i]) && !ft_isspace(s[*i]))
			(*dst)[k++] = s[(*i)++];
		(*dst)[k] = '\0';
	}
}

// Expand single argument
// It has to start from a char and end with
// a seperator
char	*expand_arg(char *s, int *i)
{
	char	*final;

	skip_redirections(s, i);
	if (s[*i] == '\0' || s[*i] == '|')
		return (NULL);
	final = ft_strdup("");
	while (s[*i] != '\0')
	{
		expand_single(s, i, &final);
		if (is_seperator(s[*i]))
			break ;
	}
	return (final);
}

char	**append_list(char **list, char *str)
{
	int		old_len;
	char	**new;
	int		i;

	old_len = 0;
	while (list[old_len])
		old_len++;
	new = malloc((old_len + 2) * sizeof(char *));
	i = 0;
	while (list[i] != NULL)
	{
		new[i] = ft_strdup(list[i]);
		free(list[i]);
		i++;
	}
	free(list);
	new[old_len] = str;
	new[old_len + 1] = NULL;
	return (new);
}

// Split the command to args, skip redirections
char	**extract_command(char *s)
{
	char	**args;
	int		i;
	char	*str;

	args = malloc(sizeof(char *));
	args[0] = NULL;
	i = 0;
	while (true)
	{
		skip_redirections(s, &i);
		str = expand_arg(s, &i);
		if (str == NULL)
			break ;
		args = append_list(args, str);
	}
	return (args);
}

void	seperate_command(char *s)
{
	int		i;
	int		j;
	int		k;
	char	current_quote;

	current_quote = '\0';
	i = -1;
	j = 0;
	g_x->cmds = ft_calloc(g_x->cmd_count, sizeof(t_command));
	while (++i < g_x->cmd_count)
		g_x->cmds[i].raw_command = ft_calloc((ft_strlen(s) + 1), sizeof(char));
	i = 0;
	while (s[i] != '\0')
	{
		k = 0;
		while (s[i] != '\0' && (s[i] != '|' || current_quote != '\0'))
		{
			the_quotes(&current_quote, s, i);
			g_x->cmds[j].raw_command[k++] = s[i++];
		}
		g_x->cmds[j].raw_command[k] = '\0';
		if (s[i] == '|')
			i++;
		j++;
	}
}
