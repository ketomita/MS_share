/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 10:20:36 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/14 11:23:09 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

static void	ft_put_command_not_found(char *command, int status)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(command, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	exit(status);
}

static void	execute_child_process(t_command_invocation *cmd, t_data *data)
{
	char	*command;
	char	*path;
	char	**current_envp;
	int		_errno;

	current_envp = convert_env_list_to_array(data->env_head);
	if (apply_redirections(cmd))
		exit(1);
	if (!cmd->exec_and_args || !cmd->exec_and_args[0])
		exit(0);
	command = (char *)cmd->exec_and_args[0];
	if (is_builtin(command) != TRANSIENT)
		exit(dispatch_builtin((char **)cmd->exec_and_args, data));
	if (ft_strchr(command, '/'))
		path = ft_strdup(command);
	else
		path = find_command_path(command, data->env_head);
	if (path == NULL)
	{
		free_string_array(current_envp);
		ft_put_command_not_found(command, 127);
	}
	execve(path, (char **)cmd->exec_and_args, current_envp);
	_errno = errno;
	ft_execve_error(path, current_envp, _errno);
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
