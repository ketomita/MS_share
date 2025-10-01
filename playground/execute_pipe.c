#include "execute.h"

static pid_t	execute_current_cmd(t_command_invocation *cmd_list, \
				pid_t *pids, t_data *data)
{
	t_fds	fds;
	pid_t	last_pid;

	fds.in_fd = STDIN_FILENO;
	last_pid = run_pipeline_commands(cmd_list, pids, data, &fds);
	if (last_pid == -1)
		return (-1);
	if (fds.in_fd != STDIN_FILENO)
		close(fds.in_fd);
	return (last_pid);
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

int	execute_pipeline(t_command_invocation *cmd_list, t_data *data)
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
	pids = allocate_pid_array(cmd_list, &cmd_count);
	if (!pids)
		return (1);
	current_cmd = cmd_list;
	last_pid = execute_current_cmd(current_cmd, pids, data);
	if (last_pid != -1)
		collect_status(cmd_count, pids, last_pid, &final_status_code);
	free(pids);
	return (final_status_code);
}
