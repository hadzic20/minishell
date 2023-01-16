#include "minishell.h"

char	status_case(char *s, int *i)
{
	char	*str;

	str = ft_itoa(g_x->error_code);
	(*i) += ft_strlen(str);
	return (str);
}

char	*dollar(char *s, int *i)
{
	int		j;
	int		check;
	char	*command;

	j = 0;
	check = 0;
	command = (char *)malloc(ft_strlen(s) * sizeof(char));
	(*i)++;
	if (s[*i] == '?')
		return (status_case(s, i));
	while (s[*i] != '\0' && s[*i] != ' ' && s[*i] != '\t' && s[*i] != '>' \
		&& s[*i] != '<' && s[*i] != '"' && s[*i] != '$')
		command[j++] = s[(*i)++];
	command[j] = '=';
	command[j + 1] = '\0';
	j = -1;
	while (++j < ft_str2len(g_x->envp) - 4)
	{
		if (ft_strnstr(g_x->envp[j], command, ft_strlen(command)))
		{
			command = ft_substr(g_x->envp[j], ft_strlen(command), ft_strlen(g_x->envp[j]));
			check = 1;
		}
	}
	if (check == 0)
		return ("");
	return (command);
}