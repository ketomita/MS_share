/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 15:26:39 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/14 15:16:41 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./parser/parser.h"
# include "./ast/ast.h"

# define GENERAL_ERROR_STATUS 1
# define BUILTIN_ERROR_STATUS 2
# define COMMAND_NOT_EXECUTABLE_STATUS 126
# define COMMAND_NOT_FOUND_STATUS 127

typedef struct s_data
{
	t_env						*env_head;
	t_env						*env_tail;
	t_command_invocation		*cmd;
	t_ast						*ast;
	t_token						*tokens;
	char						*input;
	int							stdin_backup;
	int							stdout_backup;
}								t_data;

#endif
