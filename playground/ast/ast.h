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

# include "../types.h"
# include "../lexer/lexer.h"
# include "../parser/parser.h"
# include "../expander/expander.h"
# include "../minishell.h"

typedef struct s_ast
{
	t_token_type				type;
	char						*value;
	struct s_ast				*left;
	struct s_ast				*right;
}								t_ast;

typedef struct s_cmd_redirection
{
	t_redirect_type				type;
	char						*file_path;
	int							fd;
	struct s_cmd_redirection	*next;
}								t_cmd_redirection;

typedef struct s_command_invocation
{
	t_cmd_redirection			*redirections;
	struct s_command_invocation	*piped_command;
	char						**exec_and_args;
	pid_t						pid;
}								t_command_invocation;

void					free_command_invocation(t_command_invocation *cmd);
t_command_invocation	*ast_to_command_invocation(t_data *data);
char					**create_args_array(t_data *data);
void					process_redirections(t_ast *node, \
						t_command_invocation *cmd, t_data *data);

t_cmd_redirection		*create_redirection(t_redirect_type type, \
						t_constchar *file_path);
void					add_redirection(t_cmd_redirection **head, \
						t_cmd_redirection *new_redir);
#endif
