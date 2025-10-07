/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command_handle.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 14:06:30 by hhayato           #+#    #+#             */
/*   Updated: 2025/10/07 12:56:13 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool	is_word_token(t_token_type type)
{
	return (type == EXPANDABLE || type == EXPANDABLE_QUOTED
		|| type == NON_EXPANDABLE);
}

bool	handle_cmd_redirect(t_parser *parser, t_cmd_lists *lists)
{
	t_ast	*new_node;

	new_node = parse_redirection(parser);
	if (!new_node)
	{
		free_ast(*lists->arg_head);
		return (false);
	}
	append_node(lists->redirect_head, lists->last_redirect, new_node);
	return (true);
}

bool	handle_word(t_parser *parser, t_cmd_lists *lists)
{
	t_ast	*new_node;

	new_node = create_ast_node(parser->current->type, parser->current->value);
	if (!new_node)
	{
		free_ast(*lists->arg_head);
		free_ast(*lists->redirect_head);
		return (false);
	}
	append_node(lists->arg_head, lists->last_arg, new_node);
	advance_token(parser);
	return (true);
}
