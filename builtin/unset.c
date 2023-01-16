#include "../minishell.h"

static void	unset_env(char **parse, char **name, int i)
{
	int		x;
	char	*tmp;

	x = -1;
	while (g_x->envp[++x])
	{
		*name = env_name_getter(g_x->envp[x]);
		if (ft_strcmp(*name, parse[i]) == 0)
		{
			free(*name);
			while (g_x->envp[x] && g_x->envp[x + 1])
			{
				tmp = g_x->envp[x + 1];
				g_x->envp[x + 1] = g_x->envp[x];
				g_x->envp[x++] = tmp;
			}
			if (g_x->envp[x + 1] == 0)
			{
				if (unset_env_helper(x))
					break ;
			}
		}
		if (*name)
			free(*name);
	}
}

static void	unset_export(char **parse, char **name, int i)
{
	int		x;
	char	*tmp;

	x = -1;
	while (g_x->export[++x])
	{
		*name = env_name_getter(g_x->export[x]);
		if (ft_strcmp(*name, parse[i]) == 0)
		{
			free(*name);
			while (g_x->export[x] && g_x->export[x + 1])
			{
				tmp = g_x->export[x + 1];
				g_x->export[x + 1] = g_x->export[x];
				g_x->export[x++] = tmp;
			}
			if (g_x->export[x + 1] == 0)
				if (unset_export_helper(x))
					break ;
		}
		if (*name)
			free(*name);
	}
}

void	ft_unset(char **parse)
{
	int		i;
	char	*name;

	i = 0;
	if (!parse[++i])
	{
		printf("");
		return ;
	}
	while (parse[i])
	{
		if (equal_finder(parse[i]) == 0)
		{
			unset_env(parse, &name, i);
			unset_export(parse, &name, i);
		}
		else
			unset_error(parse, i);
		i++;
	}
}