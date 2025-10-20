/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 09:58:55 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/14 11:49:40 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include <stdlib.h>

static void	ignore_signals(t_signal *signal)
{
	signal->sa_ign.sa_handler = SIG_IGN;
	sigemptyset(&signal->sa_ign.sa_mask);
	signal->sa_ign.sa_flags = 0;
	sigaction(SIGINT, &signal->sa_ign, &signal->sa_old_int);
	sigaction(SIGQUIT, &signal->sa_ign, &signal->sa_old_quit);
}

static void	restore_signals(t_signal *signal)
{
	sigaction(SIGINT, &signal->sa_old_int, NULL);
	sigaction(SIGQUIT, &signal->sa_old_quit, NULL);
}

int	execute_ast(t_command_invocation *cmd_list, t_data *data)
{
	int						status;
	t_signal				signal;

	if (!cmd_list)
		return (0);
	ignore_signals(&signal);
	status = execute_pipeline(cmd_list, data);
	restore_signals(&signal);
	return (status);
}
