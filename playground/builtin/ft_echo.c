/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 10:06:44 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/14 15:14:29 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include <unistd.h>
#include <stdbool.h>

static bool	is_n(const char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (false);
	i = 1;
	if (arg[i] == '\0')
		return (false);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

static int	check_opt(char **str)
{
	int	i;

	i = 0;
	while (str[i] && is_n(str[i]))
		i++;
	return (i);
}

int	ft_echo(char **str)
{
	int		start;
	int		i;

	start = check_opt(str);
	i = start;
	while (str[i])
	{
		ft_putstr_fd(str[i], STDOUT_FILENO);
		if (str[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (start == 0)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
