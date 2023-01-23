/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amillahadzic <amillahadzic@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 20:10:04 by amillahadzi       #+#    #+#             */
/*   Updated: 2023/01/23 15:47:07 by ykimirti         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	heredoc_handler(int sig)
{
	(void)sig;
	g_x->redirect_error = 130;
	close(0);
}

void	heredoc_loop(const char *file, int fd)
{
	char	*line;

	while (true)
	{
		line = readline("> ");
		if (line == NULL)
			break ;
		if (ft_strncmp(line, file, ft_strlen(file) + 1) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
}

int	heredoc(char *str, int *i)
{
	int		fds[2];
	void	(*mem_handler)(int);
	int		readline_dup;
	char	*file;

	(*i) += 2;
	file = expand_redirect(str, i);
	if (!file)
		return (-1);
	readline_dup = dup(0);
	if (pipe(fds) == -1)
		return (free(file), -1);
	mem_handler = signal(SIGINT, heredoc_handler);
	heredoc_loop(file, fds[1]);
	signal(SIGINT, mem_handler);
	free(file);
	dup2(readline_dup, 0);
	close(readline_dup);
	close(fds[1]);
	if (g_x->redirect_error != 0)
	{
		close(fds[0]);
		return (-1);
	}
	return (fds[0]);
}
