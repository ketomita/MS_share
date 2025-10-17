/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 14:00:15 by hhayato           #+#    #+#             */
/*   Updated: 2025/10/14 15:16:34 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "../types.h"
# include "../builtin/builtin.h"
# include "../ast/ast.h"
# include "../signal_handler.h"

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
void	ft_execve_error(char *path, char **envp, int _errno);

char	*readline_input(void);
void	parse_and_execute(char *input, t_data *data);

void	prepro_execute_child_process(t_fds *fds, \
		t_command_invocation *current_cmd, t_data *data);

int		execute_ast(t_command_invocation *cmd_list, t_data *data);

#endif
