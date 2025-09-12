#include "execute.h"

void	set_close_fd(t_fds fds, t_proctype type)
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

int	put_fork_error(t_fds fds, pid_t *pids)
{
	perror("fork");
	// TODO: エラー処理
	set_close_fd(fds, PARENTS);
	free(pids);
	return (-1);
}

void	wait_children(int cmd_count, int *status, pid_t last_pid)
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

int	check_status(int status)
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
