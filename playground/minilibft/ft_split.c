/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 10:00:20 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/08 13:46:05 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minilibft.h"
#include <stdlib.h>

static int	word_count(char c, char *str)
{
	size_t	i;
	size_t	count;
	int		flag;

	if (!str)
		return (0);
	i = 0;
	flag = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] != c && flag == 0)
		{
			count++;
			flag = 1;
		}
		else if (str[i] == c)
			flag = 0;
		i++;
	}
	return (count);
}

static void	ft_allfree(char **p, size_t i)
{
	while (0 < i)
	{
		free(p[i]);
		i--;
	}
	free(p);
	return ;
}

static char	**main_split(char *str, char c, char **p, size_t k)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		while (str[i] && str[i] == c)
			i++;
		k = i;
		while (str[i] && str[i] != c)
			i++;
		if (i > k)
		{
			p[j] = ft_strndup(str + k, i - k);
			if (!p[j])
			{
				ft_allfree(p, j);
				return (NULL);
			}
			j++;
		}
	}
	p[j] = NULL;
	return (p);
}

char	**ft_split(char *str, char c)
{
	char		**p;
	size_t		k;

	if (!str)
		return (NULL);
	p = malloc((word_count(c, str) + 1) * sizeof(char *));
	if (!p)
		return (NULL);
	k = 0;
	return (main_split(str, c, p, k));
}
