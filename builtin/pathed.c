/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathed.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amillahadzic <amillahadzic@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 20:17:11 by amillahadzi       #+#    #+#             */
/*   Updated: 2023/01/23 02:27:33 by amillahadzi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_path_from_env(void)
{
	int		i;
	int		j;
	char	**path_to_search;
	char	*str;

	i = 0;
	j = -1;
	while (++j < ft_str2len(g_x->export))
	{
		if (ft_strncmp(g_x->export[j], "PATH=", 5) == 0)
		{
			str = ft_substr(g_x->export[j], 5, ft_strlen(g_x->export[j]));
			i = 1;
			break ;
		}
	}
	if (i == 0)
		return (NULL);
	return (str);
}

char	*executable_path(char **path_to_search, char *path)
{
	int		i;
	char	*str;

	while (path_to_search[++i])
	{
		path_to_search[i] = strjoin_free(path_to_search[i], ft_strdup(path));
		if (access(path_to_search[i], X_OK) == 0)
		{
			str = ft_strdup(path_to_search[i]);
			ft_free(path_to_search);
			return (free(path), str);
		}
	}
	return (NULL);
}

char	*find_path(char *name)
{
	char	*path;
	char	**path_to_search;
	char	*str;

	if (name[0] == '.' || name[0] == '/')
		return (ft_strdup(name));
	path = ft_strjoin("/", name);
	str = get_path_from_env();
	if (str == NULL)
		return (free(path), NULL);
	path_to_search = ft_split_free(str, ':');
	str = executable_path(path_to_search, path);
	if (str)
		return (str);
	print_error("minishell", "command not found", name);
	ft_free(path_to_search);
	return (free(path), str);
}

void	mini_pathed(char **command, int outfile, int infile)
{
	char	*path;

	path = find_path(command[0]);
	if (path == NULL)
		exit(127);
	if (outfile > 1)
		dup2(outfile, 1);
	if (outfile > 1)
		close(outfile);
	if (infile > 0)
		dup2(infile, 0);
	if (infile > 0)
		close(infile);
	execve(path, command, g_x->export);
	perror("execve error");
	if (errno == 13)
		exit(126);
	exit(127);
}
