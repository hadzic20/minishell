#include "../minishell.h"

char	*status_case(char *s, int *i)
{
	char	*str;

	(void)s;
	str = ft_itoa(g_x->error_code);
	(*i)++;
	return (str);
}

char	*handle_env_substr(const char *env_var, char *command)
{
	char	*new;

	new = ft_strdup(ft_substr(env_var, ft_strlen(command), ft_strlen(env_var)));
	free(command);
	return (new);
}

// MUK
char	*dollar(char *s, int *i)
{
	int		j;
	char	*command;

	j = 0;
	command = (char *)malloc((ft_strlen(s) + 10) * sizeof(char));
	(*i)++;
	if (s[*i] == '?')
		return (free(command), status_case(s, i));
	if (!ft_isalnum(s[*i]) && s[*i] != '_')
		return (free(command), ft_strdup("$"));
	while (ft_isalnum(s[*i]) || s[*i] == '_')
		command[j++] = s[(*i)++];
	command[j] = '=';
	command[j + 1] = '\0';
	j = -1;
	while (++j < ft_str2len(g_x->export))
		if (ft_strnstr(g_x->export[j], command, ft_strlen(command)) != 0)
			return (handle_env_substr(g_x->export[j], command));
	return (ft_strdup(""));
}
