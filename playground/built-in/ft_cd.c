#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "built-in.h"

static int	change_pwd_oldpwd(char *old_pwd)
{
	char *current_pwd;

	old_pwd = getenv("OLDPWD");
	current_pwd = getcwd(NULL, 0);
	if (!current_pwd)
	{
		perror("pwd");
		return (1);
	}
	current_pwd = getenv("PWD");
	return (0);
}

int	ft_cd(char *path)
{
	char	*home_path;
	char	*joinned_path;
	char	*current_path;
	char	*buf_path;

	home_path = getenv("HOME");
	current_path = getcwd(NULL, 0);
	if (!current_path)
	{
		perror("pwd");
		return (1);
	}
	if (path == NULL)
	{
		chdir(home_path);
		change_pwd_oldpwd(current_path);
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
	if (change_pwd_oldpwd(current_path))
		return (1);
	return (0);
}

// #include <stdio.h>

// int main(int argc, char *argv[], char *envp[])
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
// 	printf("%d", ft_cd(argv[1], envp));
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
