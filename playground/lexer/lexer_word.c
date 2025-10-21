/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 14:06:18 by hhayato           #+#    #+#             */
/*   Updated: 2025/10/21 15:13:00 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <stdlib.h>

static char	*append_part(char *result, char *temp)
{
	char	*new_result;

	new_result = ft_strjoin(result, temp);
	if (!new_result)
	{
		free(result);
		free(temp);
		return (NULL);
	}
	free(result);
	free(temp);
	return (new_result);
}

static void	set_token_type(bool has_expandable, \
				bool has_quotes, t_token_type *type)
{
	if (has_expandable && has_quotes)
		*type = EXPANDABLE_QUOTED;
	else if (has_expandable && !has_quotes)
		*type = EXPANDABLE;
	else
		*type = NON_EXPANDABLE;
}

static int	process_word_loop(char *input, int pos, char **result, \
		t_expand_type *ex)
{
	char	*temp;
	int		new_pos;

	while (input[pos] && !is_whitespace(input[pos])
		&& !is_special_char(input[pos]))
	{
		new_pos = process_word_part(input, pos, &temp, ex);
		if (new_pos == -1)
			return (-1);
		*result = append_part(*result, temp);
		if (!*result)
			return (-1);
		pos = new_pos;
	}
	return (pos);
}

int	handle_concatenated_word(char *input, int pos, char **value,
		t_token_type *type)
{
	char			*result;
	t_expand_type	ex;
	int				final_pos;

	result = ft_strdup("");
	if (!result)
		return (-1);
	ex.expandable = false;
	ex.quotes = false;
	final_pos = process_word_loop(input, pos, &result, &ex);
	if (final_pos == -1)
	{
		free(result);
		return (-1);
	}
	set_token_type(ex.expandable, ex.quotes, type);
	*value = result;
	return (final_pos);
}
