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

# define COMMAND_NOT_FOUND 127

extern volatile sig_atomic_t	g_status;

typedef enum	e_builtin
{
	BUILTIN,
	BUILTIN_PARENT,
	TRANSIENT,
	NOR
}	t_builtin;

typedef enum	e_proctype
{
	PARENTS,
	CHILDREN
}	t_proctype;

typedef struct	s_fds
{
	int	in_fd;
	int	out_fd;
	int	pipe_fd[2];
}	t_fds;

int		apply_redirections(t_command_invocation *cmd);
char	*find_command_path(char *cmd, char **envp);

void	set_signal_handler(void);
void	set_parent_signal_handlers(void);

char	*readline_input(void);
int		execute_ast(t_command_invocation *cmd_list, char **envp, t_data data);
#endif
