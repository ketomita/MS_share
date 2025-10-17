/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 12:52:03 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/08 14:10:19 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "../types.h"
# include "../lexer/lexer.h"
# include "../ast/ast.h"
# include <stdbool.h>
# include <stddef.h>

typedef struct s_parser
{
	t_token						*tokens;
	t_token						*current;
}								t_parser;

typedef struct s_cmd_lists
{
	t_ast						**arg_head;
	t_ast						**redirect_head;
	t_ast						**last_arg;
	t_ast						**last_redirect;
}								t_cmd_lists;

typedef struct s_cmd_vars
{
	t_ast						*arg_head;
	t_ast						*redirect_head;
	t_ast						*last_arg;
	t_ast						*last_redirect;
}								t_cmd_vars;

void	free_ast(t_ast *node);
bool	is_redirect_token(t_token_type type);

t_ast	*parse_pipeline(t_parser *parser);
t_ast	*parse_redirection(t_parser *parser);
void	append_node(t_ast **list_head, t_ast **list_tail, t_ast *new_node);
t_ast	*create_ast_node(t_token_type type, t_constchar *value);
t_token	*advance_token(t_parser *parser);
t_ast	*parse_simple_command(t_parser *parser);

bool	is_word_token(t_token_type type);
bool	handle_cmd_redirect(t_parser *parser, t_cmd_lists *lists);
bool	handle_word(t_parser *parser, t_cmd_lists *lists);

t_ast	*parse(t_token *tokens);

#endif
