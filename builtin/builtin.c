/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amillahadzic <amillahadzic@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 20:15:53 by amillahadzi       #+#    #+#             */
/*   Updated: 2023/01/22 20:16:54 by amillahadzi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_change_dir(char *path)
{
	g_x->error_code = 0;
	if (!path || (path && *path == 0) || !ft_strncmp(path, "~", 1))
	{
		if (access(getenv("HOME"), F_OK) != -1)
			chdir(getenv("HOME"));
		else
		{
			print_error("minishell", getenv("HOME"), \
						"No such a file or directory");
			return (0);
		}
	}
	else
	{
		if (access(path, F_OK) != -1)
			chdir(path);
		else
		{
			g_x->error_code = 1;
			print_error("minishell", path, "No such a file or directory");
			return (0);
		}
	}
	return (1);
}

int	mini_pwd(int fd)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	ft_putendl_fd(pwd, fd);
	free(pwd);
	return (0);
}

void	mini_env(int fd)
{
	int	i;
	int	env_size;

	i = -1;
	env_size = ft_str2len(g_x->export);
	while (++i < env_size)
	{
		write(fd, g_x->export[i], ft_strlen(g_x->export[i]));
		write(fd, "\n", 1);
	}
}

void	mini_echo(char **parse, int fd)
{
	int		i;
	bool	newline;

	newline = true;
	i = 1;
	while (parse[i] && ft_strcmp(parse[i], "-n") == 0)
	{
		newline = false;
		i++;
	}
	while (parse[i])
	{
		ft_putstr_fd(parse[i], fd);
		if (parse[i] && parse[i + 1])
			ft_putstr_fd(" ", fd);
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", fd);
}
