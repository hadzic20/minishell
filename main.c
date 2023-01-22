/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amillahadzic <amillahadzic@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 19:57:16 by amillahadzi       #+#    #+#             */
/*   Updated: 2023/01/22 20:28:32 by amillahadzi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_x	*g_x;

void	free_them_all_mf(void)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	while (i < g_x->cmd_count)
	{
		free(g_x->cmds[i].raw_command);
		k = 0;
		while (g_x->cmds[i].handled_cmd && g_x->cmds[i].handled_cmd[k])
		{
			if (g_x->cmds)
				free(g_x->cmds[i].handled_cmd[k]);
			k++;
		}
		if (g_x->cmds[i].handled_cmd)
			free(g_x->cmds[i].handled_cmd);
		i++;
	}
	free(g_x->cmds);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;

	g_x = malloc(sizeof(t_x));
	g_x->export = storage_and_copy(envp);
	g_x->export = export_sorter(g_x->export);
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	while (argc && argv)
	{
		get_prompt();
		line = readline(g_x->prompt);
		if (!line)
			handle_crtl_d();
		g_x->cmd_count = ft_command_count(line);
		if (g_x->cmd_count != 0)
		{
			handle_line(line);
			add_history(line);
			free_them_all_mf();
		}
		free(line);
		free(g_x->prompt);
	}
}
