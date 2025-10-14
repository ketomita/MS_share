/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 10:19:22 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/14 15:18:58 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

void	free_string_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

static void	ft_put_error_and_exit(char *path, char *msg, int status)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	free(path);
	exit(status);
}

void	ft_execve_error(char *path, char **envp, int _errno)
{
	struct stat	s;

	free_string_array(envp);
	if (stat(path, &s) == 0 && S_ISDIR(s.st_mode))
		ft_put_error_and_exit(path, "Is a directory", \
			COMMAND_NOT_EXECUTABLE_STATUS);
	if (_errno == EACCES)
		ft_put_error_and_exit(path, "Permission denied", \
			COMMAND_NOT_EXECUTABLE_STATUS);
	if (_errno == ENOENT)
		ft_put_error_and_exit(path, "No such file or directory", \
			COMMAND_NOT_FOUND_STATUS);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_put_error_and_exit(path, strerror(_errno), GENERAL_ERROR_STATUS);
}
