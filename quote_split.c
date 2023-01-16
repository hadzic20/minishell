/*#include"minishell.h"

static char	*ft_quote_chr(const char *str, int c)
{
	int	i;
	unsigned char	convert_char;

	convert_char = (unsigned char)c;
	while (str[++i])
	{
		if (str[i] == '"')
		{
			i++;
			while (str[i] && str[i] != '"')
				i++;
		}
		else if (str[i] == 39)
		{
			i++;
			while (str[i] != 39 && str[i])
				i++;
			if (str[i] != 0)
				i++;
		}
	}
}*/
