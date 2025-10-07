/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 09:59:28 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/07 09:59:29 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

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

static pid_t	process_one_command(t_command_invocation *current_cmd, \
				t_fds *fds, pid_t *pids, t_data *data)
{
	pid_t	pid;

	fds->out_fd = STDOUT_FILENO;
	if (current_cmd->piped_command && prepare_pipe(fds))
	{
		free(pids);
		return (-1);
	}
	pid = fork();
	if (pid == -1)
		return (put_fork_error(pids, fds));
	if (pid == 0)
		prepro_execute_child_process(fds, current_cmd, data);
	ft_close_fd(fds, PARENTS);
	return (pid);
}

static int	check_pid(pid_t pid, t_fds *fds)
{
	if (pid == -1)
	{
		if (fds->in_fd != STDIN_FILENO)
			close(fds->in_fd);
		return (-1);
	}
	return (0);
}

pid_t	run_pipeline_commands(t_command_invocation *cmd_list, \
				pid_t *pids, t_data *data, t_fds *fds)
{
	int						i;
	pid_t					pid;
	t_command_invocation	*current_cmd;

	i = 0;
	pid = -1;
	current_cmd = cmd_list;
	while (current_cmd)
	{
		if (!current_cmd->exec_and_args || !current_cmd->exec_and_args[0])
		{
			current_cmd = current_cmd->piped_command;
			continue ;
		}
		pid = process_one_command(current_cmd, fds, pids, data);
		if (check_pid(pid, fds))
			return (-1);
		pids[i++] = pid;
		if (current_cmd->piped_command)
			fds->in_fd = fds->pipe_fd[0];
		current_cmd = current_cmd->piped_command;
	}
	pids[i] = -1;
	return (pid);
}
