#include "execute.h"

static void	free_string_array(char **array)
{
	size_t	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	ft_put_error(char *command, char *path, \
							char **env_array, t_execve_error type)
{
	if (env_array)
		free_string_array(env_array);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (type == COMMAND_NOT_FOUND)
	{
		ft_putstr_fd(command, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		exit(127);
	}
	else if (type == COMMAND_NOT_FOUND_PATH)
	{
		perror(path);
		free(path);
		exit(127);
	}
	else if (type == IS_A_DIRECTORY)
	{
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
	}
	else if (type == PERMISSION_DENIED || type == EXECVE_ERROR)
		perror(path);
	free(path);
	exit(126);
}

void	prepro_ft_put_error(char *command, \
			char *path, char **current_envp)
{
	struct stat	s;

	if (!path)
		ft_put_error(command, path, current_envp, COMMAND_NOT_FOUND);
	if (access(path, F_OK) != 0)
		ft_put_error(command, path, current_envp, COMMAND_NOT_FOUND_PATH);
	if (stat(path, &s) == 0 && S_ISDIR(s.st_mode))
		ft_put_error(command, path, current_envp, IS_A_DIRECTORY);
	if (access(path, X_OK) != 0)
		ft_put_error(command, path, current_envp, PERMISSION_DENIED);
}
