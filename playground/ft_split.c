#include "execute.h"

static int	word_count(char c, char *str)
{
	int	i;
	int	count;
	int	flag;

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

static char	*ft_strndup(char *str, int j)
{
	int		i;
	char	*p;

	i = 0;
	p = malloc(j + 1);
	if (!p)
		return (NULL);
	i = 0;
	while (i < j)
	{
		p[i] = str[i];
		i++;
	}
	p[i] = '\0';
	return (p);
}

static void	ft_allfree(char **p, int i)
{
	while (0 <= i)
	{
		free(p[i]);
		i--;
	}
	free(p);
	return ;
}

static char	**main_split(char *str, char c, char **p, int k)
{
	int	i;
	int	j;

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
	char	**p;
	int		k;

	if (!str)
		return (NULL);
	p = malloc((word_count(c, str) + 1) * sizeof(char *));
	if (!p)
		return (NULL);
	k = 0;
	return (main_split(str, c, p, k));
}
