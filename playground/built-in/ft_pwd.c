#include "builtin.h"

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
