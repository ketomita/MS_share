/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 10:06:44 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/07 10:06:45 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static int	check_opt(char **str)
{
	int	i;

	i = 0;
	while (str[i] && ft_strcmp(str[i], "-n") == 0)
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
			write(1, " ", STDOUT_FILENO);
		i++;
	}
	if (start == 0)
		write(1, "\n", STDOUT_FILENO);
	return (0);
}
