/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_user_and_path.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amillahadzic <amillahadzic@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 20:31:00 by amillahadzi       #+#    #+#             */
/*   Updated: 2023/01/22 20:38:28 by amillahadzi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_user(void)
{
	int		i;
	int		check;
	char	*user;

	i = -1;
	check = 0;
	while (g_x->export[++i])
	{
		if (ft_strnstr(g_x->export[i], "USER=", 5))
		{
			user = ft_substr(g_x->export[i], 5, ft_strlen(g_x->export[i]));
			check = 1;
			break ;
		}
	}
	if (check == 0)
		return ("guest");
	return (user);
}

char	*get_path(void)
{
	char	*home;
	char	*path;
	char	*tmppath;

	home = getenv("HOME");
	tmppath = getcwd(NULL, 0);
	path = ft_substr(tmppath, ft_strlen(home), ft_strlen(tmppath));
	free(tmppath);
	return (path);
}

void	get_prompt(void)
{
	char	*user;
	char	*path;
	char	*temp;
	char	*temp2;

	user = get_user();
	temp = ft_strjoin(user, "@minishell ~");
	free(user);
	path = get_path();
	temp2 = ft_strjoin(temp, path);
	free(path);
	free(temp);
	g_x->prompt = ft_strjoin(temp2, " % ");
	free(temp2);
}
