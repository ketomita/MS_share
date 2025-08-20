#include <stdlib.h>

#include "minilibft.h"

char	*ft_strdup(const char *s1)
{
	char	*copy;
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s1[count])
		count++;
	copy = (char *)malloc(sizeof(char) * (count + 1));
	if (copy == NULL)
		return (NULL);
	while (i <= count)
	{
		copy[i] = s1[i];
		i++;
	}
	return (copy);
}

char	*ft_strndup(const char *s1, size_t n)
{
	char	*copy;
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s1[count] && count < n)
		count++;
	copy = (char *)malloc(sizeof(char) * (count + 1));
	if (copy == NULL)
		return (NULL);
	while (i <= count)
	{
		copy[i] = s1[i];
		i++;
	}
	copy[count] = '\0';
	return (copy);
}
