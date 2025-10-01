#include "execute.h"

static void	execute_child_process(t_command_invocation *cmd, t_data *data)
{
	char		*command;
	char		*path;
	char		**current_envp;

	current_envp = convert_env_list_to_array(data->env_head);
	if (apply_redirections(cmd))
		exit(1);
	if (is_builtin(cmd->exec_and_args[0]) != TRANSIENT)
		exit(dispatch_builtin((char **)cmd->exec_and_args, data));
	command = (char *)cmd->exec_and_args[0];
	if (ft_strchr(command, '/'))
		path = ft_strdup(command);
	else
		path = find_command_path(command, data->env_head);
	prepro_ft_put_error(command, path, current_envp);
	execve(path, (char **)cmd->exec_and_args, current_envp);
	ft_put_error(command, path, current_envp, EXECVE_ERROR);
}

static void	reset_default_signal(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
}

void	prepro_execute_child_process(t_fds *fds, \
		t_command_invocation *current_cmd, t_data *data)
{
	if (current_cmd->piped_command)
		close(fds->pipe_fd[0]);
	reset_default_signal();
	ft_close_fd(fds, CHILDREN);
	execute_child_process(current_cmd, data);
}
