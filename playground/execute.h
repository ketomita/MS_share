#ifndef EXECUTE_H
# define EXECUTE_H

# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <fcntl.h>
# include <wait.h>
# include <errno.h>

# include "lexer_parser.h"

void	set_signal_handler(void);
char	*readline_input(void);

int		execute_ast(t_ast *node, char **envp);

#endif
