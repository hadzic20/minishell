/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amillahadzic <amillahadzic@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 20:27:08 by amillahadzi       #+#    #+#             */
/*   Updated: 2023/01/22 20:27:33 by amillahadzi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**ft_split_free(char *str, char c)
{
	char	**res;

	res = ft_split(str, c);
	free(str);
	return (res);
}

char	*ft_substr_free(char *str, int start, int end)
{
	char	*res;

	res = ft_substr(str, start, end);
	free(str);
	return (res);
}

char	*strjoin_free(char *a, char *b)
{
	char	*new;

	new = ft_strjoin(a, b);
	free(a);
	free(b);
	return (new);
}
