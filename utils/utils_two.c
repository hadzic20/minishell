/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amillahadzic <amillahadzic@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 20:27:12 by amillahadzi       #+#    #+#             */
/*   Updated: 2023/01/22 20:27:13 by amillahadzi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	*ft_realloc(void *ptr, size_t size)
{
	void	*temp;

	temp = malloc(sizeof(char) * size);
	ft_strlcpy(temp, ptr, size);
	free(ptr);
	return (temp);
}

char	**ft_rrealloc(char **ptr, size_t size)
{
	char	**temp;
	int		i;

	i = -1;
	temp = malloc(sizeof(char *) * (size + 1));
	while (ptr[++i])
	{
		temp[i] = malloc(sizeof(char) * (ft_strlen(ptr[i]) + 1));
		temp[i] = ft_memcpy(temp[i], ptr[i], ft_strlen(ptr[i]) + 1);
	}
	temp[i] = 0;
	ft_free(ptr);
	return (temp);
}

void	ft_free(char **str)
{
	int	i;

	i = -1;
	while (str[++i])
		free(str[i]);
	free(str);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t			t;
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	if (str2)
	{
		t = 0;
		while (str1[t] != '\0' && str2[t] != '\0' && str1[t] == str2[t])
			t++;
		return (str1[t] - str2[t]);
	}
	return (1);
}
