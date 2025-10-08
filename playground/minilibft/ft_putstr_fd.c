/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 10:01:45 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/08 13:45:56 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minilibft.h"
#include <unistd.h>
#include <stdlib.h>

void	ft_putstr_fd(char *s, int fd)
{
	size_t	len;
	ssize_t	bytes_written;

	if (s == NULL)
		return ;
	len = ft_strlen(s);
	while (len > 0)
	{
		bytes_written = write(fd, s, len);
		if (bytes_written == -1)
			return ;
		len -= bytes_written;
		s += bytes_written;
	}
}
