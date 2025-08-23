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
	if (!current_path)
	{
		perror("cd: getcwd");
		return (1);
	}
	pwd = ft_strjoin("PWD=", current_path);
	ft_export(data, pwd);
	free(current_path);
	free(pwd);
	return (0);
}

void	set_home_or_target_path(char *path, \
	char **target_path, int *result, int mode)
{
	char	*home_path;

	if (mode == 0)
	{
		*target_path = getenv("HOME");
		if (!(*target_path))
		{
			ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
			*result = 1;
		}
	}
	else if (mode == 1)
	{
		home_path = getenv("HOME");
		if (!home_path)
		{
			ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
			*result = 1;
		}
		else
			*target_path = ft_strjoin(home_path, path + 1);
	}
}

void	set_target_path(t_data *data, \
	char *path, char **target_path, int *result)
{
	t_env	*env_buf;

	if (path == NULL)
		set_home_or_target_path(path, target_path, result, 0);
	else if (*path == '~')
		set_home_or_target_path(path, target_path, result, 1);
	else if (ft_strcmp(path, "-") == 0)
	{
		env_buf = find_env_node(data->env_head, "OLDPWD");
		if (!env_buf)
		{
			ft_putstr_fd("cd: OLDPWD not set\n", STDERR_FILENO);
			*result = 1;
		}
		else
			*target_path = env_buf->value;
	}
	else
		*target_path = path;
}

int	ft_cd(t_data *data, char *path)
{
	char	*old_path;
	char	*target_path;
	int		result;

	result = 0;
	old_path = getcwd(NULL, 0);
	if (!old_path)
	{
		perror("cd: getcwd");
		return (1);
	}
	set_target_path(data, path, &target_path, &result);
	if (result == 0 && chdir(target_path) != 0)
	{
		perror("cd");
		result = 1;
	}
	if (result == 0 && change_pwd_oldpwd(data, old_path))
		result = 1;
	free(old_path);
	if (path && *path == '~')
		free(target_path);
	if (path && ft_strcmp(path, "-") == 0 && result == 0)
		ft_pwd();
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
