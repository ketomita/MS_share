#include "execute.h"

t_builtin	is_builtin(const char *cmd)
{
	if (cmd == NULL)
		return (NOR);
	if (ft_strcmp(cmd, "echo") == 0)
		return (BUILTIN);
	if (ft_strcmp(cmd, "cd") == 0)
		return (BUILTIN_PARENT);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (BUILTIN);
	if (ft_strcmp(cmd, "export") == 0)
		return (BUILTIN_PARENT);
	if (ft_strcmp(cmd, "unset") == 0)
		return (BUILTIN_PARENT);
	if (ft_strcmp(cmd, "env") == 0)
		return (BUILTIN);
	if (ft_strcmp(cmd, "exit") == 0)
		return (BUILTIN_PARENT);
	return (TRANSIENT);
}

static void	ft_put_error(char *command, char *path, t_execve_error type)
{
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

static void	execute_child_process(t_command_invocation *cmd, \
			char **envp, t_data data)
{
	char		*command;
	char		*path;
	struct stat	s;
	t_builtin	builtin_type;

	if (apply_redirections(cmd))
		exit(1);
	builtin_type = is_builtin(cmd->exec_and_args[0]);
	if (builtin_type == BUILTIN)
		exit(dispatch_builtin((char **)cmd->exec_and_args, data));
	if (builtin_type == BUILTIN_PARENT)
	{
		if (ft_strcmp(cmd->exec_and_args[0], "exit") == 0)
			dispatch_builtin((char **)cmd->exec_and_args, data);
		exit(0);
	}
	command = (char *)cmd->exec_and_args[0];
	if (ft_strchr(command, '/'))
		path = ft_strdup(command);
	else
		path = find_command_path(command, data.env_head);
	if (!path)
		ft_put_error(command, path, COMMAND_NOT_FOUND);
	if (access(path, F_OK) != 0)
		ft_put_error(command, path, COMMAND_NOT_FOUND_PATH);
	if (stat(path, &s) == 0 && S_ISDIR(s.st_mode))
		ft_put_error(command, path, IS_A_DIRECTORY);
	if (access(path, X_OK) != 0)
		ft_put_error(command, path, PERMISSION_DENIED);
	execve(path, (char **)cmd->exec_and_args, envp);
	ft_put_error(command, path, EXECVE_ERROR);
}

static void	reset_default_signal(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
}

void	prepro_execute_child_process(t_fds fds, \
	t_command_invocation *current_cmd, char **envp, t_data data)
{
	if (current_cmd->piped_command)
		close(fds.pipe_fd[0]);
	reset_default_signal();
	ft_close_fd(fds, CHILDREN);
	execute_child_process(current_cmd, envp, data);
}
