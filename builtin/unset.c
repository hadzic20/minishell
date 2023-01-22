/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amillahadzic <amillahadzic@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 20:12:41 by amillahadzi       #+#    #+#             */
/*   Updated: 2023/01/22 20:12:42 by amillahadzi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	unset_export(char **parse, char **name, int i)
{
	int		x;
	char	*tmp;

	x = -1;
	while (g_x->export[++x])
	{
		*name = env_name_getter(g_x->export[x]);
		if (ft_strcmp(*name, parse[i]) == 0)
		{
			free(*name);
			while (g_x->export[x] && g_x->export[x + 1])
			{
				tmp = g_x->export[x + 1];
				g_x->export[x + 1] = g_x->export[x];
				g_x->export[x++] = tmp;
			}
			if (g_x->export[x + 1] == 0)
				if (unset_export_helper(x))
					break ;
		}
		if (*name)
			free(*name);
	}
}

void	ft_unset(char **parse)
{
	int		i;
	char	*name;

	i = 0;
	if (!parse[++i])
		return ;
	while (parse[i])
	{
		if (equal_finder(parse[i]) == 0)
		{
			unset_export(parse, &name, i);
		}
		else
			unset_error(parse, i);
		i++;
	}
}
