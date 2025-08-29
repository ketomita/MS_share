#include "execute.h"

extern volatile sig_atomic_t g_status;

static void	set_status(int status)
{
	g_status = status;
}

static void	handle_sigint(int signo)
{
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	set_status(128 + signo);
}

void	set_signal_handler(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint;
	if (sigemptyset(&sa.sa_mask))
	{
		perror("sigemptyset");
		exit(EXIT_FAILURE);
	}
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL))
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
	sa.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sa, NULL))
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
}

char	*readline_input(void)
{
	char	*line;

	line = readline("minishell$ ");
	if (line == NULL)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		return (NULL);
	}
	if (ft_strlen(line) > 0)
		add_history(line);
	return (line);
}
