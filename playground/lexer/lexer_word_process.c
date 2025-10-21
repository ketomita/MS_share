/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word_process.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 14:06:15 by hhayato           #+#    #+#             */
/*   Updated: 2025/10/21 15:12:39 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static int	process_quoted_part(char *input, int pos, \
				char **temp, t_expand_type *ex)
{
	t_token_type	temp_type;
	int				new_pos;

	new_pos = handle_quotes(input, pos, temp, &temp_type);
	if (new_pos == -1 || !*temp)
		return (-1);
	ex->quotes = true;
	if (temp_type == EXPANDABLE_QUOTED)
		ex->expandable = true;
	return (new_pos);
}

static int	process_unquoted_part(char *input, int pos, \
				char **temp, bool *has_expandable)
{
	int	start_pos;

	start_pos = pos;
	while (input[pos] && !is_whitespace(input[pos])
		&& !is_special_char(input[pos]) && !is_quote(input[pos]))
		pos++;
	*temp = ft_substr(input, start_pos, pos - start_pos);
	if (!*temp)
		return (-1);
	if (ft_strchr(*temp, '$'))
		*has_expandable = true;
	return (pos);
}

int	process_word_part(char *input, int pos, char **temp, t_expand_type *ex)
{
	int	new_pos;

	*temp = NULL;
	if (is_quote(input[pos]))
	{
		new_pos = process_quoted_part(input, pos, temp, ex);
		if (new_pos == -1)
			return (-1);
	}
	else
	{
		new_pos = process_unquoted_part(input, pos, temp, &ex->expandable);
		if (new_pos == -1)
			return (-1);
	}
	return (new_pos);
}
