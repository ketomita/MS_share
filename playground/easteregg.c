/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   easteregg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 10:21:31 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/07 10:21:31 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "easter.h"
#include <fcntl.h>
#include <unistd.h>

static int	write_nyancat(int fd)
{
	int		bytes_read;
	char	buf[42];

	while (1)
	{
		bytes_read = read(fd, buf, 42);
		if (bytes_read <= 0)
			break ;
		if (write(STDOUT_FILENO, buf, bytes_read) != bytes_read)
		{
			ft_putstr_fd("nyancat: write error\n", STDERR_FILENO);
			close(fd);
			return (1);
		}
	}
	close(fd);
	if (bytes_read == -1)
	{
		ft_putstr_fd("nyancat: read error\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

int	is_nyancat(const char *str)
{
	int		fd;

	if (ft_strcmp(str, "nyancat") != 0)
		return (0);
	fd = open("./nyancat", O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("nyancat: ./nyancat: No such file or directory\n", 2);
		g_status = 1;
		return (1);
	}
	g_status = write_nyancat(fd);
	return (1);
}
