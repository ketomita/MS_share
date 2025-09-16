#include "execute.h"

void	ft_close_fd(t_fds fds, t_proctype type)
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

int	put_fork_error(pid_t *pids)
{
	int	i;

	perror("minishell: fork");
	i = 0;
	while (pids[i] != -1)
	{
		kill(pids[i], SIGKILL);
		i++;
	}
	free(pids);
	return (-1);
}

int	check_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGPIPE)
		{
			ft_putstr_fd(" Broken pipe\n", STDERR_FILENO);
		}
		else if (WTERMSIG(status) == SIGINT || WTERMSIG(status) == SIGQUIT)
		{
			//なんらかの処理を入れる可能性あり
		}
		return (128 + WTERMSIG(status));
	}
	return (status);
}

void	wait_and_collect_statuses(int cmd_count, pid_t *pids, t_child_status *statuses)
{
	int i;

	i = 0;
	while (i < cmd_count)
	{
		statuses[i].pid = waitpid(pids[i], &statuses[i].status, 0);
		i++;
	}
}

pid_t	*prepare_pids(t_command_invocation *current_cmd, int *cmd_count)
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
