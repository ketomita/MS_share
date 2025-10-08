/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 14:05:59 by hhayato           #+#    #+#             */
/*   Updated: 2025/10/08 13:49:32 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "../minilibft/minilibft.h"
# include <stdbool.h>
# include <signal.h>

extern volatile sig_atomic_t	g_status;

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

typedef enum e_redirect_type
{
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}								t_redirect_type;

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

typedef struct s_env
{
	struct s_env				*prev;
	char						*name;
	char						*value;
	struct s_env				*next;
}								t_env;

typedef struct s_expand_ctx
{
	char						*str;
	char						*result;
	int							i;
	int							j;
	t_env						*env_list;
}								t_expand_ctx;

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

int		process_word_part(char *input, int pos, \
		char **temp, bool *has_expandable);
int		handle_quotes(char *input, int pos, char **value, t_token_type *type);
char	*handle_multiline_input(char *initial_input);

t_token	*tokenize(char *input);

#endif
