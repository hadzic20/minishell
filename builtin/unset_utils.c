#include "../minishell.h"

char	**ft_rrealloc_with_skip(char **ptr, int index_to_skip)
{
	char	**temp;
	int		i;

	temp = malloc(sizeof(char *) * ft_str2len(g_x->export) + 1);
	i = -1;
	while (ptr[++i])
	{
		if (i == index_to_skip)
			continue ;
		temp[i] = malloc(sizeof(char) * (ft_strlen(ptr[i]) + 1));
		temp[i] = ft_memcpy(temp[i], ptr[i], ft_strlen(ptr[i]) + 1);
	}
	temp[i] = 0;
	ft_free(ptr);
	return (temp);
}

int	unset_export_helper(int x)
{
	free(g_x->export[x]);
	g_x->export[x] = 0;
	g_x->export = ft_rrealloc_with_skip(g_x->export, x);
	return (1);
}

void	unset_error(char **parse, int i)
{
	printf("minishell: unset: `%s': not a valid identifier\n", parse[i]);
}