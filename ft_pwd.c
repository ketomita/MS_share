#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

int	ft_pwd(void)
{
	char	*pathname;

	pathname = getcwd(NULL, 0);
	if (!pathname)
	{
		perror("pwd");
		return (1);
	}
	write(1, pathname, ft_strlen(pathname));
	write(1, "\n", 1);
	free(pathname);
	return (0);
}

// int main(void)
// {
// 	return (ft_pwd());
// }
