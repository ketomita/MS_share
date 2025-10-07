/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 09:58:55 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/07 09:58:58 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

static char	**rebuild_arguments(int non_empty_count, char **args)
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

static char	**remove_empty_arguments(char **args)
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
	return (rebuild_arguments(non_empty_count, args));
}

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
	t_command_invocation	*current;

	if (!cmd_list)
		return (0);
	current = cmd_list;
	while (current)
	{
		current->exec_and_args = remove_empty_arguments \
		((char **)current->exec_and_args);
		current = current->piped_command;
	}
	ignore_signals(&signal);
	status = execute_pipeline(cmd_list, data);
	restore_signals(&signal);
	return (status);
}
