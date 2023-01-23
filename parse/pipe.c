/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykimirti <ykimirti@42istanbul.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 16:18:44 by ykimirti          #+#    #+#             */
/*   Updated: 2023/01/23 16:36:23 by ykimirti         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_line_utils(int i, int save_fd)
{
	if (i != 0)
	{
		dup2(save_fd, 0);
		close(save_fd);
	}
	close(g_x->cmds[i].p[0]);
	if (i != g_x->cmd_count - 1)
		dup2(g_x->cmds[i].p[1], 1);
	close(g_x->cmds[i].p[1]);
	execute_command(&g_x->cmds[i], true);
}

int	fork_inside_pipe(int i, int save_fd)
{
	int	pid;

	pid = fork();
	if (pid == 0)
	{
		handle_line_utils(i, save_fd);
		exit(g_x->error_code);
	}
	return (pid);
}

int	get_status(int pid)
{
	int	status;

	status = 0;
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

/*
 * Executes from g_x->cmds and their raw_commands
 * Sets the error_code to the status of the last command.
 * If there are any errors on redirect or errors while parsing,
 * it does not continue to execute it
 */
void	handle_pipe(void)
{
	int	i;
	int	save_fd;
	int	last_pid;

	i = 0;
	while (i < g_x->cmd_count)
	{
		if (!redirect(&g_x->cmds[i]))
			break ;
		pipe(g_x->cmds[i].p);
		last_pid = fork_inside_pipe(i, save_fd);
		close(g_x->cmds[i].p[1]);
		if (i != 0)
			close(save_fd);
		save_fd = g_x->cmds[i].p[0];
		i++;
	}
	if (i != 0)
	{
		close(save_fd);
		g_x->error_code = get_status(last_pid);
	}
}
