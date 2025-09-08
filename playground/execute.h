#ifndef EXECUTE_H
# define EXECUTE_H

# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <fcntl.h>
# include <wait.h>
# include <errno.h>

# include "lexer_parser.h"
# include "minilibft.h"
# include "builtin.h"

extern volatile sig_atomic_t	g_status;

void	set_signal_handler(void);
char	*readline_input(void);
int		execute_ast(t_command_invocation *cmd_list, char **envp, t_data data);
#endif
