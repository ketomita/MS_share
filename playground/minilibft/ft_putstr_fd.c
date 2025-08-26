#include "minilibft.h"

void	ft_putstr_fd(char *s, int fd)
{
	size_t		len;
	size_t		buf1;
	size_t		buf2;
	size_t		i;

	if (!s)
		return ;
	len = ft_strlen(s);
	i = 0;
	if (len > INT_MAX)
	{
		buf1 = len / INT_MAX;
		buf2 = len % INT_MAX;
		while (i < buf1)
		{
			write(fd, s, INT_MAX);
			s += INT_MAX;
			i++;
		}
		write(fd, s, buf2);
		return ;
	}
	write(fd, s, len);
	return ;
}
