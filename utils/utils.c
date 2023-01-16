#include "../minishell.h"

int	ft_word_count(char *str)
{
	int	i;
	int	size;

	i = 0;
	size = 0;
	while (str[i] != '\0')
	{
		if (str[i] != ' ' && str[i] != '\t' && \
		(str[i + 1] == ' ' || str[i + 1] == '\0'))
			size++;
		i++;
	}
	return (size);
}

int	ft_command_count(char *str)
{
	int	i;
	int	size;

	i = 0;
	size = 1;
	while (str[i] != '\0')
	{
		if (str[i] == '|' && str[i + 1] != '|')
			size++;
		i++;
	}
	return (size);
}

int	ft_str2len(char **str2)
{
	int	i;

	i = 0;
	while (str2[i])
		i++;
	return (i);
}

void	fill_envp(char **envp)
{
	int	i;

	i = -1;
	g_x->envp = malloc((ft_str2len(envp) + 1) * sizeof(char *));
	while (++i < ft_str2len(envp))
		g_x->envp[i] = ft_strdup(envp[i]);
	g_x->envp[i] = ft_strdup("\0");
}

void	free_envp(void)
{
	int	i;

	i = 0;
	while (g_x->envp && g_x->envp[i])
	{
		free(g_x->envp[i]);
		i++;
	}
    //free(g_x->prompt);
	free(g_x->envp);
	free(g_x);
}
//char **free_envp()
