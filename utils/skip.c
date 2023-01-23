#include "../minishell.h"

void	skip_arg(char *s, int *i)
{
	while (!is_seperator(s[*i]))
		(*i)++;
}

void	skip_spaces(char *s, int *i)
{
	while (ft_isspace(s[*i]))
		(*i)++;
}

// Respect pipes and other redirection symbols here >:(
void	skip_redirection(char *s, int *i)
{
	if (s[*i] != '<' && s[*i] != '>')
		return ;
	(*i)++;
	if (s[*i] == '<' || s[*i] == '>')
		(*i)++;
	skip_spaces(s, i);
	skip_arg(s, i);
}

void	skip_quote(char *s, int *i)
{
	char	quote;

	quote = s[*i];
	(*i)++;
	while (s[*i] != quote)
	{
		if (s[*i] == '\0')
			return ;
		(*i)++;
	}
	(*i)++;
}
