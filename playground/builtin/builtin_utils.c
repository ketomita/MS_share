/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 10:06:49 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/08 13:53:25 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include <stdlib.h>
#include <readline/readline.h>

size_t	count_builtin_args(char **args)
{
	size_t	i;

	if (!args)
		return (0);
	i = 0;
	while (args[i])
		i++;
	return (i);
}

int	is_valid_identifier(const char *name)
{
	if (!name || (!ft_isalpha(*name) && *name != '_'))
		return (0);
	name++;
	while (*name)
	{
		if (!ft_isalnum(*name) && *name != '_')
			return (0);
		name++;
	}
	return (1);
}

int	dispatch_builtin(char **args, t_data *data)
{
	const char	*cmd;

	cmd = args[0];
	if (ft_strcmp(cmd, "echo") == 0)
		return (ft_echo(args + 1));
	if (ft_strcmp(cmd, "cd") == 0)
		return (ft_cd(data, args));
	if (ft_strcmp(cmd, "pwd") == 0)
		return (ft_pwd());
	if (ft_strcmp(cmd, "export") == 0)
		return (ft_export(data, args));
	if (ft_strcmp(cmd, "unset") == 0)
		return (ft_unset(data, args));
	if (ft_strcmp(cmd, "env") == 0)
		return (ft_env(data->env_head));
	if (ft_strcmp(cmd, "exit") == 0)
		return (ft_exit(data, args));
	return (1);
}

void	free_all_resources(t_data *data)
{
	free_env_list(data->env_head);
	free_command_invocation(data->cmd);
	free_ast(data->ast);
	free_tokens(data->tokens);
	free(data->input);
	rl_clear_history();
}

bool	is_builtin(char *cmd)
{
	if (!cmd)
		return (false);
	if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0 || \
		ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0 || \
		ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0 || \
		ft_strcmp(cmd, "exit") == 0)
		return (true);
	return (false);
}
