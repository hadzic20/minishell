#include "minishell.h"

void	print_list(char **list)
{
	int	i;

	i = 0;
	while (list[i] != NULL)
		printf("> %s\n", list[i++]);
}