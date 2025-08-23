#ifndef EXECUTE_H
# define EXECUTE_H

# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <fcntl.h>
# include <wait.h>
# include <errno.h>

# include "lexer_parser.h"

typedef enum	e_builtin
{
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT
}	t_builtin;

void	set_signal_handler(void);
char	*readline_input(void);

int		execute_ast(t_ast *node, char **envp);

#endif
