/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 10:53:46 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/21 10:53:46 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_HANDLER_H
# define SIGNAL_HANDLER_H

# include <signal.h>
# include <unistd.h>

extern volatile sig_atomic_t	g_signal;

typedef struct s_signal
{
	struct sigaction			sa_ign;
	struct sigaction			sa_old_int;
	struct sigaction			sa_old_quit;
}								t_signal;

void	set_signal_handler(void);
void	set_parent_signal_handlers(void);
void	set_child_signal_handlers(void);

#endif
