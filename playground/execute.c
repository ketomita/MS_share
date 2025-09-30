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

static pid_t	create_children()
{
	pid_t	pid;

	fds.out_fd = STDOUT_FILENO;
	if (current_cmd->piped_command && prepare_pipe(&fds))
		return (free(pids), -1);
	pid = fork();
	if (pid == -1)
		return (ft_close_fd(fds, PARENTS), put_fork_error(pids));
	if (pid == 0)
		prepro_execute_child_process(fds, current_cmd, data);
	return (pid);
}

static pid_t	execute_current_cmd(t_command_invocation *current_cmd, \
				pid_t *pids, t_data *data)
{
	t_fds	fds;
	int		i;
	pid_t	pid;

	i = 0;
	pid = -1;
	fds.in_fd = STDIN_FILENO;
	while (current_cmd)
	{
		if (!current_cmd->exec_and_args || !current_cmd->exec_and_args[0])
		{
			current_cmd = current_cmd->piped_command;
			continue ;
		}

		pids[i++] = pid;
		ft_close_fd(fds, PARENTS);
		if (current_cmd->piped_command)
			fds.in_fd = fds.pipe_fd[0];
		current_cmd = current_cmd->piped_command;
	}
	if (fds.in_fd != STDIN_FILENO)
		close(fds.in_fd);
	pids[i] = -1;
	return (pid);
}

int	preprocess_heredocs(t_command_invocation *cmd_list)
{
	t_command_invocation	*current_cmd;
	t_cmd_redirection		*redir;

	current_cmd = cmd_list;
	while (current_cmd)
	{
		redir = current_cmd->redirections;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
			{
				redir->fd = handle_heredoc(redir->file_path);
				if (redir->fd == -1)
					return (-1);
			}
			redir = redir->next;
		}
		current_cmd = current_cmd->piped_command;
	}
	return (0);
}

static int	save_last_cmd_status(int cmd_count, \
			t_child_status *statuses, pid_t last_pid)
{
	int	i;
	int	last_cmd_status;

	i = 0;
	last_cmd_status = 0;
	while (i < cmd_count)
	{
		if (statuses[i].pid == last_pid)
		{
			last_cmd_status = statuses[i].status;
			break ;
		}
		i++;
	}
	return (last_cmd_status);
}

static void	collect_status(int cmd_count, pid_t *pids, \
			pid_t last_pid, int *final_status_code)
{
	int				last_cmd_status;
	int				i;
	int				last_cmd_failed;
	t_child_status	*statuses;

	statuses = malloc(sizeof(t_child_status) * cmd_count);
	wait_and_collect_statuses(cmd_count, pids, statuses);
	last_cmd_status = save_last_cmd_status(cmd_count, statuses, last_pid);
	last_cmd_failed = (WIFSIGNALED(last_cmd_status) || \
	(WIFEXITED(last_cmd_status) && WEXITSTATUS(last_cmd_status) != 0));
	i = 0;
	while (i < cmd_count)
	{
		if (!last_cmd_failed && WIFSIGNALED(statuses[i].status))
			check_status(statuses[i].status);
		i++;
	}
	free(statuses);
	*final_status_code = check_status(last_cmd_status);
}

static int	execute_pipeline(t_command_invocation *cmd_list, t_data *data)
{
	t_command_invocation	*current_cmd;
	pid_t					*pids;
	pid_t					last_pid;
	int						cmd_count;
	int						final_status_code;

	final_status_code = 0;
	if (preprocess_heredocs(cmd_list) != 0)
		return (1);
	if (cmd_list && !cmd_list->piped_command && \
		is_builtin(cmd_list->exec_and_args[0]) == BUILTIN_PARENT)
		return (execute_builtin(cmd_list, data));
	current_cmd = cmd_list;
	cmd_count = 0;
	pids = prepare_pids(current_cmd, &cmd_count);
	if (!pids)
		return (1);
	current_cmd = cmd_list;
	last_pid = execute_current_cmd(current_cmd, pids, data);
	if (last_pid != -1)
		collect_status(cmd_count, pids, last_pid, &final_status_code);
	free(pids);
	return (final_status_code);
}

static char	**rebuild_new_args(int non_empty_count, char **args)
{
	char	**new_args;
	int		i;
	int		j;

	new_args = (char **)malloc(sizeof(char *) * (non_empty_count + 1));
	if (!new_args)
		return (NULL);
	i = 0;
	j = 0;
	while (args[i])
	{
		if (*args[i] != '\0')
			new_args[j++] = args[i];
		else
			free(args[i]);
		i++;
	}
	new_args[j] = NULL;
	free(args);
	return (new_args);
}

static char	**rebuild_args_without_empty_strings(char **args)
{
	int		i;
	int		non_empty_count;

	if (!args)
		return (NULL);
	non_empty_count = 0;
	i = 0;
	while (args[i])
	{
		if (*args[i] != '\0')
			non_empty_count++;
		i++;
	}
	return (rebuild_new_args(non_empty_count, args));
}

int	execute_ast(t_command_invocation *cmd_list, t_data *data)
{
	int						status;
	struct sigaction		sa_ign;
	struct sigaction		sa_old_int;
	struct sigaction		sa_old_quit;
	t_command_invocation	*current_cmd;

	if (!cmd_list)
		return (0);
	current_cmd = cmd_list;
	while (current_cmd)
	{
		current_cmd->exec_and_args = (const char **) \
		rebuild_args_without_empty_strings((char **)current_cmd->exec_and_args);
		current_cmd = current_cmd->piped_command;
	}
	sa_ign.sa_handler = SIG_IGN;
	sigemptyset(&sa_ign.sa_mask);
	sa_ign.sa_flags = 0;
	sigaction(SIGINT, &sa_ign, &sa_old_int);
	sigaction(SIGQUIT, &sa_ign, &sa_old_quit);
	status = execute_pipeline(cmd_list, data);
	sigaction(SIGINT, &sa_old_int, NULL);
	sigaction(SIGQUIT, &sa_old_quit, NULL);
	return (status);
}
