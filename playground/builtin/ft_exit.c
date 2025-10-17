/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 10:07:09 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/14 15:11:33 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <limits.h>

static bool	is_numeric_string(const char *str)
{
	if (!str || !*str)
		return (true);
	if (*str == '+' || *str == '-')
		str++;
	if (!*str)
		return (true);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (true);
		str++;
	}
	return (false);
}

static bool	is_longlong_overflow(const char *str)
{
	long long	result;
	int			sign;
	int			digit;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		digit = *str - '0';
		if (sign == 1)
		{
			if (result > LLONG_MAX / 10 || \
				(result == LLONG_MAX / 10 && digit > LLONG_MAX % 10))
				return (true);
		}
		else if (result > -(LLONG_MIN / 10) || \
				(result == -(LLONG_MIN / 10) && digit > -(LLONG_MIN % 10)))
			return (true);
		result = result * 10 + digit;
		str++;
	}
	return (false);
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
		exit_minishell(data, data->exit_status);
	if (is_numeric_string(args[1]) || is_longlong_overflow(args[1]))
		put_exit_error(data, args[1], BUILTIN_ERROR_STATUS);
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
