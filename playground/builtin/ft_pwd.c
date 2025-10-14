/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 10:07:18 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/14 15:13:00 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int	ft_pwd(void)
{
	char	*pathname;

	pathname = getcwd(NULL, 0);
	if (!pathname)
	{
		perror("pwd");
		return (1);
	}
	write(STDOUT_FILENO, pathname, ft_strlen(pathname));
	write(STDOUT_FILENO, "\n", 1);
	free(pathname);
	return (0);
}
