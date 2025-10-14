/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 14:00:15 by hhayato           #+#    #+#             */
/*   Updated: 2025/10/14 09:58:10 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "builtin.h"
# include "minishell.h"

# define STATUS_COMMAND_NOT_FOUND 127

extern volatile sig_atomic_t	g_status;

typedef enum e_proctype
{
	PARENTS,
	CHILDREN
}								t_proctype;

typedef struct s_fds
{
	int							in_fd;
	int							out_fd;
	int							pipe_fd[2];
}								t_fds;

typedef struct s_child_status
{
	pid_t						pid;
	int							status;
}								t_child_status;

typedef enum e_execve_error
{
	COMMAND_NOT_FOUND,
	COMMAND_NOT_FOUND_PATH,
	IS_A_DIRECTORY,
	PERMISSION_DENIED,
	EXECVE_ERROR
}								t_execve_error;

typedef struct s_signal
{
	struct sigaction			sa_ign;
	struct sigaction			sa_old_int;
	struct sigaction			sa_old_quit;
}								t_signal;

typedef enum e_main_error
{
	TOKENS,
	AST,
	CMD
}								t_main_error;

int		execute_pipeline(t_command_invocation *cmd_list, t_data *data);
int		apply_redirections(t_command_invocation *cmd);
pid_t	run_pipeline_commands(t_command_invocation *cmd_list, \
				pid_t *pids, t_data *data, t_fds *fds);

void	ft_close_fd(t_fds *fds, t_proctype type);
int		put_fork_error(pid_t *pids, t_fds *fds);
int		check_status(int status);
void	wait_and_collect_statuses(int cmd_count, pid_t *pids, \
		t_child_status *statuses);
pid_t	*allocate_pid_array(t_command_invocation *cmd_list, int *cmd_count);

int		preprocess_heredocs(t_command_invocation *cmd_list);
int		execute_builtin(t_command_invocation *cmd, t_data *data);

char	*find_command_path(char *cmd, t_env *env_list);

void	free_string_array(char **array);
void	ft_put_error_and_exit(char *command, char *msg, int status);
void	ft_execve_error(char *path, int _errno);

void	set_signal_handler(void);
void	set_parent_signal_handlers(void);

char	*readline_input(void);
void	parse_and_execute(char *input, t_data *data);

void	prepro_execute_child_process(t_fds *fds, \
		t_command_invocation *current_cmd, t_data *data);

int		execute_ast(t_command_invocation *cmd_list, t_data *data);

#endif
