#include "minishell.h"

char	*get_user(void)
{
	int		i;
	int		check;
	char	*user;

	i = -1;
	check = 0;
	while (g_x->envp[++i])
	{
		if (ft_strnstr(g_x->envp[i], "USER=", 5))
		{
			user = ft_substr(g_x->envp[i], 5, ft_strlen(g_x->envp[i]));
			check = 1;
			break ;
		}
	}
	if (check == 0)
		return ("guest");
	return (user);
}

char	*get_path(void)
{
	int		i;
	char	*home;
	char	*path;

	home = getenv("HOME");
	path = getcwd(NULL, 0);
	path = ft_substr(path, ft_strlen(home), ft_strlen(path));
	return (path);
}

void	get_prompt(void)
{
	char	*user;
	char	*path;
	char	*temp;
	char	*temp2;

	user = get_user();
	temp = ft_strjoin(user, "@minishell ~");
	free(user);
	path = get_path();
	temp2 = ft_strjoin(temp, path);
	free(path);
	free(temp);
	g_x->prompt = ft_strjoin(temp2, " % ");
	free(temp2);
}