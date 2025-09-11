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

static int	dispatch_builtin(char **args, t_data data)
{
	const char	*cmd;

	cmd = args[0];
	if (ft_strcmp(cmd, "echo") == 0)
		return (ft_echo(args + 1));
	if (ft_strcmp(cmd, "cd") == 0)
		return (ft_cd(&data, args));
	if (ft_strcmp(cmd, "pwd") == 0)
		return (ft_pwd());
	if (ft_strcmp(cmd, "export") == 0)
		return (ft_export(&data, args[1]));
	if (ft_strcmp(cmd, "unset") == 0)
		return (ft_unset(&data, args[1]));
	if (ft_strcmp(cmd, "env") == 0)
		return (ft_env(data.env_head));
	if (ft_strcmp(cmd, "exit") == 0)
		return (ft_exit(&data, args));
	return (1);
}

static void	restore_fds(int stdin_backup, int stdout_backup)
{
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
}

static int	execute_builtin(t_command_invocation *cmd, t_data data)
{
	int	stdin_backup;
	int	stdout_backup;
	int	result;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (apply_redirections(cmd) == -1)
	{
		restore_fds(stdin_backup, stdout_backup);
		return (1);
	}
	result = dispatch_builtin((char **)cmd->exec_and_args, data);
	restore_fds(stdin_backup, stdout_backup);
	return (result);
}

static void	set_default_signal(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
}

static void	set_close_fd(t_fds fds, t_proctype type)
{
	if (fds.in_fd != STDIN_FILENO)
	{
		if (type == CHILDREN)
			dup2(fds.in_fd, STDIN_FILENO);
		close(fds.in_fd);
	}
	if (fds.out_fd != STDOUT_FILENO)
	{
		if (type == CHILDREN)
			dup2(fds.out_fd, STDOUT_FILENO);
		close(fds.out_fd);
	}
}

static void	execute_child_process(t_command_invocation *cmd, \
			char **envp, t_data data)
{
	char	*path;

	if (apply_redirections(cmd) == -1)
		exit(1);
	if (is_builtin(cmd->exec_and_args[0]) == BUILTIN || \
		is_builtin(cmd->exec_and_args[0]) == BUILTIN_PARENT)
		exit(dispatch_builtin((char **)cmd->exec_and_args, data));
	else
	{
		path = find_command_path((char *)cmd->exec_and_args[0], envp);
		if (path)
		{
			execve(path, (char **)cmd->exec_and_args, envp);
			free(path);
		}
		perror((char *)cmd->exec_and_args[0]);
		exit(COMMAND_NOT_FOUND);
	}
}

static int	prepare_pipe(t_fds *fds, pid_t *pids)
{
	if (pipe(fds->pipe_fd) == -1)
	{
		perror("pipe");
		free(pids);
		return (-1);
	}
	fds->out_fd = fds->pipe_fd[1];
	return (0);
}

static int	put_fork_error(t_fds fds, pid_t *pids)
{
	perror("fork");
	// TODO: エラー処理
	set_close_fd(fds, PARENTS);
	free(pids);
	return (-1);
}

static void	prepro_execute_child_process(t_fds fds, \
			t_command_invocation *current_cmd, char **envp, t_data data)
{
	if (current_cmd->piped_command)
		close(fds.pipe_fd[0]);
	set_default_signal();
	set_close_fd(fds, CHILDREN);
	execute_child_process(current_cmd, envp, data);
}

static pid_t	execute_current_cmd(t_command_invocation *current_cmd, \
				pid_t *pids, char **envp, t_data data)
{
	t_fds	fds;
	int		i;
	pid_t	pid;

	i = 0;
	fds.in_fd = STDIN_FILENO;
	if (!current_cmd)
		return (-1);
	while (current_cmd)
	{
		fds.out_fd = STDOUT_FILENO;
		if (current_cmd->piped_command && prepare_pipe(&fds, pids))
			return (-1);
		pid = fork();
		if (pid == -1)
			return (put_fork_error(fds, pids));
		if (pid == 0)
			prepro_execute_child_process(fds, current_cmd, envp, data);
		pids[i++] = pid;
		set_close_fd(fds, PARENTS);
		if (current_cmd->piped_command)
			fds.in_fd = fds.pipe_fd[0];
		current_cmd = current_cmd->piped_command;
	}
	return (pid);
}

static void	wait_children(int cmd_count, int *status, pid_t last_pid)
{
	int	i;
	int	waited_pid;
	int	child_status;

	i = 0;
	while (i < cmd_count)
	{
		waited_pid = waitpid(-1, &child_status, 0);
		if (waited_pid == last_pid)
			*status = child_status;
		if (waited_pid == -1)
		{
			if (errno != ECHILD)
				perror("waitpid");
			break ;
		}
		i++;
	}
}

static int	check_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGPIPE)
		{
			ft_putstr_fd("Broken pipe\n", STDERR_FILENO);
		}
		else if (WTERMSIG(status) == SIGINT || WTERMSIG(status) == SIGQUIT)
		{
			//なんらかの処理を入れる可能性あり
		}
		return (128 + WTERMSIG(status));
	}
	return (status);
}

static pid_t	*prepare_pids(t_command_invocation *current_cmd, int *cmd_count)
{
	pid_t	*pids;

	while (current_cmd)
	{
		(*cmd_count)++;
		current_cmd = current_cmd->piped_command;
	}
	pids = malloc(sizeof(pid_t) * (*cmd_count));
	if (!pids)
	{
		perror("malloc");
		return (NULL);
	}
	return (pids);
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
	wait_children(cmd_count, &status, last_pid);
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
