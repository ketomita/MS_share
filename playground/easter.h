/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   easter.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 09:59:49 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/07 09:59:49 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EASTER_H
# define EASTER_H
# include <fcntl.h>
# include <unistd.h>
# include <signal.h>
# include "minilibft.h"

extern volatile sig_atomic_t	g_status;

int	is_nyancat(const char *str);
#endif
