#include "../minishell.h"

t_x *g_x;

char	**storage_and_copy(char **env)
{
	int		i;
	int		x;
	char	**str;

	str = ft_calloc(sizeof(char *), ft_str2len(env) + 1);
	i = -1;
	while (env[++i])
		str[i] = ft_calloc(sizeof(char), ft_strlen(env[i]) + 1);
	i = -1;
	while (env[++i])
	{
		x = -1;
		while (env[i][++x])
			str[i][x] = env[i][x];
	}
	return (str);
}

char	**export_sorter(char **envp)
{
	int		i;
	char	*tmp;

	i = 0;
	while (envp[i])
	{
		if (envp[i + 1] && ft_strncmp(envp[i], envp[i + 1], ft_strlen(envp[i]) + 1) > 0)
		{
			tmp = envp[i];
			envp[i] = envp[i + 1];
			envp[i + 1] = tmp;
			i = -1;
		}
		i++;
	}
	return (envp);
}

int	equal_finder(char *var)
{
	int	i;

	i = -1;
	while (var[++i])
	{
		if (var[i] == '=')
			return (1);
	}
	return (0);
}

// ahmet=mehmet ->  ahmet
char	*env_name_getter(char *var)
{
	int		i;
	char	*name;

	i = 0;
	while (var[i] != '=' && var[i])
		i++;
	name = ft_calloc(sizeof(char), (i + 1));
	i = -1;
	while (var[++i] != '=' && var[i])
		name[i] = var[i];
	name[i] = 0;
	return (name);
}

char	*env_getter(char *str)
{
	int		i;
	int		x;
	int		len;
	int		a;
	char	*temp;

	temp = NULL;
	a = 0;
	x = 0;
	i = 0;
	len = 0;
	while (str[i] != '=')
		i++;
	if (str[i] == '=')
	{
		a = i++;
		while (str[a++])
			len++;
		temp = ft_calloc(sizeof(char), len);
		while (str[i])
			temp[x++] = str[i++];
	}
	return (temp);
}
