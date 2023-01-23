/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amillahadzic <amillahadzic@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 20:22:04 by amillahadzi       #+#    #+#             */
/*   Updated: 2023/01/23 16:38:31 by ykimirti         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_line(char *str)
{
	seperate_command(str);
	if (g_x->cmd_count == 1)
	{
		if (!redirect(&g_x->cmds[0]))
			return ;
		execute_command(&g_x->cmds[0], false);
	}
	else
		handle_pipe();
	while (waitpid(-1, NULL, 0) != -1)
		;
}
