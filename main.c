/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amillahadzic <amillahadzic@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 19:57:16 by amillahadzi       #+#    #+#             */
/*   Updated: 2023/01/23 16:54:20 by ykimirti         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_x	*g_x;

void	free_them_all_mf(void)
{
	int	i;

	i = 0;
	while (i < g_x->cmd_count)
	{
		if (g_x->cmds[i].raw_command != NULL)
			free(g_x->cmds[i].raw_command);
		if (g_x->cmds[i].handled_cmd != NULL)
			ft_free(g_x->cmds[i].handled_cmd);
		i++;
	}
	free(g_x->cmds);
}

void	testdq(char *str)
{
	int		i;
	char	*final;

	i = 0;
	final = double_quote(str, &i);
	printf("@%s@\t->\t@%s@\n", str, final);
	free(final);
}

void	testextract(char *str)
{
	char	**final;

	printf("=== @%s@ ===\n", str);
	final = extract_command(str);
	print_list(final);
	ft_free(final);
}

void	test(char *str)
{
	int		i;
	char	*final;

	i = 0;
	final = expand_arg(str, &i);
	printf("@%s@\t->\t@%s@ i: %d\n", str, final, i);
	free(final);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;

	g_x = ft_calloc(1, sizeof(t_x));
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
