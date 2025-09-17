#ifndef EXECUTE_H
# define EXECUTE_H

# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <fcntl.h>
# include <wait.h>
# include <errno.h>
# include <sys/stat.h>

# include "lexer_parser.h"
# include "minilibft.h"
# include "builtin.h"

# define COMMAND_NOT_FOUND 127

extern volatile sig_atomic_t	g_status;

typedef enum e_builtin
{
	BUILTIN,
	BUILTIN_PARENT,
	TRANSIENT,
	NOR
}	t_builtin;

typedef enum e_proctype
{
	PARENTS,
	CHILDREN
}	t_proctype;

typedef struct s_fds
{
	int	in_fd;
	int	out_fd;
	int	pipe_fd[2];
}	t_fds;

typedef struct s_child_status
{
	pid_t	pid;
	int		status;
}	t_child_status;

int		handle_heredoc(const char *delimiter);

int		apply_redirections(t_command_invocation *cmd);
char	*find_command_path(char *cmd, char **envp);

void	set_signal_handler(void);
void	set_parent_signal_handlers(void);

void	prepro_execute_child_process(t_fds fds, \
		t_command_invocation *current_cmd, char **envp, t_data data);

int		execute_builtin(t_command_invocation *cmd, t_data data);

char	*readline_input(void);
int		execute_ast(t_command_invocation *cmd_list, char **envp, t_data data);

pid_t	*prepare_pids(t_command_invocation *current_cmd, int *cmd_count);
int		check_status(int status);
void	wait_and_collect_statuses(int cmd_count, \
		pid_t *pids, t_child_status *statuses);
int		put_fork_error(pid_t *pids);
void	ft_close_fd(t_fds fds, t_proctype type);

#endif
