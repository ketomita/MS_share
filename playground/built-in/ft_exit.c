/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 10:07:09 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/07 12:04:12 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static bool	is_numeric_string(const char *str)
{
	if (!str || !*str)
		return (false);
	if (*str == '+' || *str == '-')
		str++;
	if (!*str)
		return (false);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (false);
		str++;
	}
	return (true);
}

static void	exit_minishell(t_data *data, int status)
{
	if (isatty(STDIN_FILENO))
		ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (data->stdin_backup != -1)
		close(data->stdin_backup);
	if (data->stdout_backup != -1)
		close(data->stdout_backup);
	free_all_resources(data);
	rl_clear_history();
	exit((unsigned char)status);
}

static void	put_exit_error(t_data *data, char *str, int exit_status)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	exit_minishell(data, exit_status);
}

int	ft_exit(t_data *data, char **args)
{
	int			arg_count;
	long long	status;

	arg_count = count_builtin_args(args);
	if (arg_count == 1)
		exit_minishell(data, g_status);
	if (!is_numeric_string(args[1]))
		put_exit_error(data, args[1], 2);
	if (arg_count > 2)
	{
		if (isatty(STDIN_FILENO))
			ft_putstr_fd("exit\n", STDOUT_FILENO);
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	status = ft_atoll(args[1]);
	exit_minishell(data, status);
	return (0);
}
