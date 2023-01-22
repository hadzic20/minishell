/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amillahadzic <amillahadzic@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 20:11:44 by amillahadzi       #+#    #+#             */
/*   Updated: 2023/01/22 20:11:49 by amillahadzi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**ft_rrealloc_with_skip(char **ptr, int index_to_skip)
{
	char	**temp;
	int		i;
	int		tmp_index;

	temp = malloc(sizeof(char *) * (ft_str2len(g_x->export) - 1 + 1));
	i = -1;
	while (ptr[++i])
	{
		if (i == index_to_skip)
			continue ;
		if (i > index_to_skip)
			tmp_index = i - 1;
		else
			tmp_index = i;
		temp[tmp_index] = malloc(sizeof(char) * (ft_strlen(ptr[i]) + 1));
		temp[tmp_index] = ft_memcpy(temp[tmp_index],
				ptr[i], ft_strlen(ptr[i]) + 1);
	}
	temp[tmp_index + 1] = NULL;
	ft_free(ptr);
	return (temp);
}

int	unset_export_helper(int x)
{
	g_x->export = ft_rrealloc_with_skip(g_x->export, x);
	return (1);
}

void	unset_error(char **parse, int i)
{
	printf("minishell: unset: `%s': not a valid identifier\n", parse[i]);
	g_x->error_code = 1;
}
