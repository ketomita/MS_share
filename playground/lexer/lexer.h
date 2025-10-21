/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 14:05:59 by hhayato           #+#    #+#             */
/*   Updated: 2025/10/21 15:11:40 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "../types.h"
# include "../minilibft/minilibft.h"
# include <stdbool.h>
# include <signal.h>

typedef const char				t_constchar;

typedef enum e_token_type
{
	EXPANDABLE,
	EXPANDABLE_QUOTED,
	NON_EXPANDABLE,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	REDIRECT_APPEND,
	REDIRECT_HEREDOC,
	NODE_COMMAND,
	EOF_TOKEN
}	t_token_type;

typedef struct s_token
{
	t_token_type				type;
	char						*value;
	struct s_token				*next;
}								t_token;

typedef struct s_expand_ctx
{
	char						*str;
	char						*result;
	int							i;
	int							j;
	t_env						*env_list;
}								t_expand_ctx;

typedef struct s_expand_type
{
	bool						expandable;
	bool						quotes;
}								t_expand_type;

void	free_tokens(t_token *head);
int		handle_redirect(char *input, int pos, char **value, t_token_type *type);
int		handle_concatenated_word(char *input, \
		int pos, char **value, t_token_type *type);
t_token	*create_token(t_token_type type, t_constchar *value);
void	add_token(t_token **head, t_token *new_token);

int		skip_whitespace(char *str, int pos);
bool	is_quote(char c);
bool	is_whitespace(char c);
bool	is_special_char(char c);

int		process_word_part(char *input, int pos, char **temp, t_expand_type *ex);
int		handle_quotes(char *input, int pos, char **value, t_token_type *type);
char	*handle_multiline_input(char *initial_input);

t_token	*tokenize(char *input);

#endif
