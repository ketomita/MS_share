#include <stdlib.h>

#include "minilibft.h"

char	*ft_strdup(const char *s1)
{
	char	*copy;
	size_t	i;
	size_t	len;

	i = 0;
	len = 0;
	while (s1[len])
		len++;
	copy = (char *)malloc(sizeof(char) * (len + 1));
	if (copy == NULL)
		return (NULL);
	while (i <= len)
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
	size_t	len;

	i = 0;
	len = 0;
	while (s1[len] && len < n)
		len++;
	copy = (char *)malloc(sizeof(char) * (len + 1));
	if (copy == NULL)
		return (NULL);
	while (i <= len)
	{
		copy[i] = s1[i];
		i++;
	}
	copy[len] = '\0';
	return (copy);
}
