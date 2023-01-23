/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykimirti <ykimirti@42istanbul.com.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 16:01:51 by ykimirti          #+#    #+#             */
/*   Updated: 2023/01/23 16:10:11 by ykimirti         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Only use this function to execute ANY command.
 * If it is in a fork, it does not fork again for execve
 * It executes waitpid ONLY IF the there is single command
 * and its not a builtin.
 *
 * If you are doing this in a fork, you have to set the error_code
 * yourself
 */
void	execute_command(t_command *cmd, bool is_in_fork)
{
	cmd->handled_cmd = extract_command(cmd->raw_command);
	g_x->error_code = 0;
	if (cmd->handled_cmd[0] == NULL)
		return ;
	handle_command(cmd->handled_cmd, cmd->outfile,
		cmd->infile, is_in_fork);
}

void	handle_nonbuiltin(char **command, int outfd, int infd, bool is_in_fork)
{
	int	pid;
	int	status;

	if (is_in_fork)
		return (mini_pathed(command, outfd, infd));
	pid = fork();
	if (pid == 0)
		mini_pathed(command, outfd, infd);
	status = 0;
	waitpid(pid, &status, 0);
	g_x->error_code = WEXITSTATUS(status);
}

void	handle_command(char **command, int outfd, int infd, bool is_in_fork)
{
	if (command == NULL)
		return ;
	if (ft_strncmp(command[0], "cd", 3) == 0)
		ft_change_dir(command[1]);
	else if (ft_strncmp(command[0], "pwd", 4) == 0)
		mini_pwd(outfd);
	else if (ft_strncmp(command[0], "env", 4) == 0)
		mini_env(outfd);
	else if (ft_strncmp(command[0], "echo", 5) == 0)
		mini_echo(command, outfd);
	else if (ft_strncmp(command[0], "export", 7) == 0)
		ft_export(command, outfd);
	else if (ft_strncmp(command[0], "unset", 6) == 0)
		ft_unset(command);
	else if (ft_strncmp(command[0], "exit", 5) == 0)
		ft_exit(command);
	else
		handle_nonbuiltin(command, outfd, infd, is_in_fork);
}
