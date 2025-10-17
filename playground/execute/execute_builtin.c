/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 10:22:35 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/08 13:57:36 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include <unistd.h>

static void	restore_fds(int stdin_backup, int stdout_backup)
{
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
}

int	execute_builtin(t_command_invocation *cmd, t_data *data)
{
	int	result;

	data->stdin_backup = dup(STDIN_FILENO);
	data->stdout_backup = dup(STDOUT_FILENO);
	if (apply_redirections(cmd))
	{
		restore_fds(data->stdin_backup, data->stdout_backup);
		data->stdin_backup = -1;
		data->stdout_backup = -1;
		return (1);
	}
	result = dispatch_builtin((char **)cmd->exec_and_args, data);
	restore_fds(data->stdin_backup, data->stdout_backup);
	data->stdin_backup = -1;
	data->stdout_backup = -1;
	return (result);
}
