#include "minilibft.h"

#include <stdint.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	void			*p;
	size_t			total;

	if (size != 0 && nmemb > SIZE_MAX / size)
		return (NULL);
	total = nmemb * size;
	p = malloc(total);
	if (!p)
		return (NULL);
	ft_bzero(p, total);
	return (p);
}
