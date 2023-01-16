#include "../minishell.h"

int	unset_env_helper(int x)
{
	free(g_x->envp[x]);
	g_x->envp[x] = 0;
	//g_x->envp = ft_rrealloc(g_x->envp, ft_str2len(g_x->envp));
	return (1);
}

int	unset_export_helper(int x)
{
	free(g_x->export[x]);
	g_x->export[x] = 0;
	g_x->export = ft_rrealloc(g_x->export, \
					ft_str2len(g_x->export));
	return (1);
}

void	unset_error(char **parse, int i)
{
	printf("minishell: unset: `%s': not a valid identifier\n", parse[i]);
}