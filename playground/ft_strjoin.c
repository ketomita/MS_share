#include "execute.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*p;
	size_t	size_s1;
	size_t	size_s2;

	if (!s1)
		return (NULL);
	size_s1 = ft_strlen(s1);
	size_s2 = ft_strlen(s2);
	p = malloc(sizeof(char) * (size_s1 + size_s2 + 1));
	if (!p)
		return (NULL);
	ft_memmove(p, s1, size_s1);
	ft_memmove(p + size_s1, s2, size_s2);
	p[size_s1 + size_s2] = '\0';
	return (p);
}
