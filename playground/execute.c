#include "execute.h"

static t_builtin	is_builtin(const char *cmd)
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

static int	prepare_pipe(t_fds *fds)
{
	if (pipe(fds->pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	fds->out_fd = fds->pipe_fd[1];
	return (0);
}

static pid_t	execute_current_cmd(t_command_invocation *current_cmd, \
				pid_t *pids, char **envp, t_data data)
{
	t_fds	fds;
	int		i;
	pid_t	pid;

	i = 0;
	pid = -1;
	fds.in_fd = STDIN_FILENO;
	while (current_cmd)
	{
		if (!current_cmd->exec_and_args || !current_cmd->exec_and_args[0] || \
			!*((char *)current_cmd->exec_and_args[0]))
		{
			current_cmd = current_cmd->piped_command;
			continue;
		}
		fds.out_fd = STDOUT_FILENO;
		if (current_cmd->piped_command && prepare_pipe(&fds))
			return (free(pids), -1);
		pid = fork();
		if (pid == -1)
		{
			if (fds.in_fd != STDIN_FILENO) close(fds.in_fd);
			if (fds.out_fd != STDOUT_FILENO) close(fds.out_fd);
			return (put_fork_error(pids));
		}
		if (pid == 0)
			prepro_execute_child_process(fds, current_cmd, envp, data);
		pids[i++] = pid;
		if (fds.in_fd != STDIN_FILENO)
			close(fds.in_fd);
		if (fds.out_fd != STDOUT_FILENO)
			close(fds.out_fd);
		if (current_cmd->piped_command)
			fds.in_fd = fds.pipe_fd[0];
		current_cmd = current_cmd->piped_command;
	}
	if (fds.in_fd != STDIN_FILENO)
		close(fds.in_fd);
	pids[i] = -1;
	return (pid);
}

static int	execute_pipeline(t_command_invocation *cmd_list, \
			char **envp, t_data data)
{
	int						status;
	t_command_invocation	*current_cmd;
	pid_t					*pids;
	pid_t					last_pid;
	int						cmd_count;

	if (cmd_list && !cmd_list->piped_command && \
		is_builtin(cmd_list->exec_and_args[0]) == BUILTIN_PARENT)
		return (execute_builtin(cmd_list, data));
	current_cmd = cmd_list;
	cmd_count = 0;
	pids = prepare_pids(current_cmd, &cmd_count);
	if (!pids)
		return (1);
	status = 0;
	current_cmd = cmd_list;
	last_pid = execute_current_cmd(current_cmd, pids, envp, data);
	if (last_pid != -1)
		wait_children(cmd_count, pids, &status, last_pid);
	free(pids);
	return (check_status(status));
}

int	execute_ast(t_command_invocation *cmd_list, char **envp, t_data data)
{
	int					status;
	struct sigaction	sa_ign;
	struct sigaction	sa_old_int;
	struct sigaction	sa_old_quit;

	if (!cmd_list)
		return (0);
	sa_ign.sa_handler = SIG_IGN;
	sigemptyset(&sa_ign.sa_mask);
	sa_ign.sa_flags = 0;
	sigaction(SIGINT, &sa_ign, &sa_old_int);
	sigaction(SIGQUIT, &sa_ign, &sa_old_quit);
	status = execute_pipeline(cmd_list, envp, data);
	sigaction(SIGINT, &sa_old_int, NULL);
	sigaction(SIGQUIT, &sa_old_quit, NULL);
	return (status);
}
