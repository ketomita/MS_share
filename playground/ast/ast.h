/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 15:25:47 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/07 15:25:48 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "../parser/parser.h"
# include "../expander/expander.h"
# include "../minilibft/minilibft.h"

void					free_command_invocation(t_command_invocation *cmd);
t_command_invocation	*ast_to_command_invocation(t_ast *ast, t_env *env_list);
char					**create_args_array(t_ast *node, t_env *env_list);
void					process_redirections(t_ast *node, \
						t_command_invocation *cmd, t_env *env_list);

t_cmd_redirection		*create_redirection(t_redirect_type type, \
						t_constchar *file_path);
void					add_redirection(t_cmd_redirection **head, \
						t_cmd_redirection *new_redir);
#endif
