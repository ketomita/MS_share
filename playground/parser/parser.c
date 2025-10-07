/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 14:06:55 by hhayato           #+#    #+#             */
/*   Updated: 2025/10/07 12:56:43 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_ast	*parse(t_token *tokens)
{
	t_parser	parser;
	t_ast		*ast;

	if (!tokens)
		return (NULL);
	parser.tokens = tokens;
	parser.current = tokens;
	if (parser.current->type == EOF_TOKEN)
		return (NULL);
	ast = parse_pipeline(&parser);
	return (ast);
}
