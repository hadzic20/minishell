#include "minishell.h"

t_x	*g_x;

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
		if (line)
		{
			handle_line(line);
			add_history(line);
			free(line);
		}
		free(g_x->prompt);
	}
	//system("leaks a.out");
}
