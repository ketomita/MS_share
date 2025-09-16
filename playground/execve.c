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

static void	execute_child_process(t_command_invocation *cmd, \
			char **envp, t_data data)
{
	char		*command;
	char		*path;
	struct stat	s;
	t_builtin	builtin_type;

	if (apply_redirections_input(cmd) || apply_redirections_output(cmd))
		exit(1);
	builtin_type = is_builtin(cmd->exec_and_args[0]);
	if (builtin_type == BUILTIN)
		exit(dispatch_builtin((char **)cmd->exec_and_args, data));
	command = (char *)cmd->exec_and_args[0];
	if (ft_strchr(command, '/'))
		path = ft_strdup(command);
	else
		path = find_command_path(command, envp);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(command, 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	if (access(path, F_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(path);
		free(path);
		exit(127);
	}
	if (stat(path, &s) == 0 && S_ISDIR(s.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		free(path);
		exit(126);
	}
	if (access(path, X_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(path);
		free(path);
		exit(126);
	}
	execve(path, (char **)cmd->exec_and_args, envp);
	ft_putstr_fd("minishell: ", 2);
	perror(path);
	free(path);
	exit(126);
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
