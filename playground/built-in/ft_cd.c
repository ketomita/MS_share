#include "builtin.h"

static int	change_pwd_oldpwd(t_data *data, char *old_path)
{
	char	*pwd;
	char	*oldpwd;
	char	*current_path;

	oldpwd = ft_strjoin("OLDPWD=", old_path);
	ft_export(data, oldpwd);
	free(oldpwd);
	current_path = getcwd(NULL, 0);
	pwd = ft_strjoin("PWD=", current_path);
	ft_export(data, pwd);
	free(current_path);
	free(pwd);
	return (0);
}

int ft_cd(t_data *data, char *path)
{
	char    *old_path;
	char    *target_path;
	char    *home_path;
	int     result;

	target_path = NULL;
	result = 0;
	old_path = getcwd(NULL, 0);
	if (!old_path)
	{
		perror("cd: getcwd");
		return (1);
	}
	if (path == NULL)
	{
		target_path = getenv("HOME");
		if (!target_path)
		{
			ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
			result = 1;
		}
	}
	else if (*path == '~')
	{
		home_path = getenv("HOME");
		if (!home_path)
		{
			ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
			result = 1;
		}
		else
			target_path = ft_strjoin(home_path, path + 1);
	}
	else
		target_path = path;
	if (result == 0)
	{
		if (chdir(target_path) != 0)
		{
			perror("cd");
			result = 1;
		}
	}
	if (result == 0)
	{
		if (change_pwd_oldpwd(data, old_path))
			result = 1;
	}
	free(old_path);
	if (path && *path == '~')
		free(target_path);
	return (result);
}

// #include <stdio.h>

// int main(int argc, char *argv[], char *envp[])
// {
// 	char *pathname;
// 	t_data data;

// 	init_env_list(&data, envp);
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
