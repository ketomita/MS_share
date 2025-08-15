#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "built-in.h"

static void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t	i;

	if (!dest && !src)
		return (NULL);
	i = 0;
	if (dest < src)
	{
		while (i < n)
		{
			((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
			i++;
		}
	}
	else
	{
		while (0 < n)
		{
			n--;
			((unsigned char *)dest)[n] = ((unsigned char *)src)[n];
		}
	}
	return (dest);
}

static char	*ft_strjoin(char const *s1, char const *s2)
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

int ft_cd(char *path)
{
	char	*home_path;
	char	*joinned_path;
	char	*current_path;
	char	*buf_path;

	home_path = getenv("HOME");
	if (path == NULL)
	{
		chdir(home_path);
		return (0);
	}
	if (*path == '/')
	{
		if (chdir(path))
		perror("cd");
		return (1);
	}
	else if (*path == '~')
	{
		path++;
		joinned_path = ft_strjoin(home_path, path);
		if (chdir(joinned_path))
		{
			perror("cd");
			return (1);
		}
		free(joinned_path);
	}
	else
	{
		current_path = getcwd(NULL, 0);
		if (!current_path)
		{
			perror("pwd");
			return (1);
		}
		buf_path = ft_strjoin(current_path, "/");
		joinned_path = ft_strjoin(buf_path, path);
		free(buf_path);
		if (chdir(joinned_path))
		{
			perror("cd");
			free(joinned_path);
			return (1);
		}
		free(joinned_path);
	}
	return (0);
}

// #include <stdio.h>

// int main(int argc, char *argv[])
// {
// 	char *pathname;

// 	pathname = getcwd(NULL, 0);
// 	if (!pathname)
// 	{
// 		perror("pwd");
// 		return (1);
// 	}
// 	write(1, pathname, ft_strlen(pathname));
// 	write(1, "\n", 1);
// 	free(pathname);
// 	printf("%d", ft_cd(argv[1]));
// 	pathname = getcwd(NULL, 0);
// 	if (!pathname)
// 	{
// 		perror("pwd");
// 		return (1);
// 	}
// 	write(1, pathname, ft_strlen(pathname));
// 	write(1, "\n", 1);
// 	free(pathname);
// 	return (0);
// }
