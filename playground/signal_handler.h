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

#endif