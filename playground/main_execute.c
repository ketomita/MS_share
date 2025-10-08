/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_execute.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 12:40:23 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/08 14:06:37 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include <stdlib.h>
#include <unistd.h>

static void	put_error_and_free(t_ast *ast, t_token *tokens, \
			char *input, t_main_error type)
{
	if (type == TOKENS)
	{
		ft_putstr_fd("Tokenization failed!\n", STDERR_FILENO);
		free(input);
	}
	else if (type == AST)
	{
		ft_putstr_fd("Parsing failed!\n", STDERR_FILENO);
		free_tokens(tokens);
		free(input);
	}
	else if (type == CMD)
	{
		ft_putstr_fd("AST to command_invocation conversion failed!\n", \
					STDERR_FILENO);
		free_ast(ast);
		free_tokens(tokens);
		free(input);
	}
}

static void	run_and_cleanup(t_data *data, t_command_invocation *cmd)
{
	data->cmd = cmd;
	g_status = execute_ast(cmd, data);
	free_command_invocation(cmd);
	free_ast(data->ast);
	free_tokens(data->tokens);
	free(data->input);
}

void	parse_and_execute(char *input, t_data *data)
{
	t_command_invocation	*cmd;

	data->input = input;
	data->tokens = tokenize(input);
	if (!data->tokens)
	{
		put_error_and_free(NULL, NULL, input, TOKENS);
		return ;
	}
	data->ast = parse(data->tokens);
	if (!data->ast)
	{
		put_error_and_free(NULL, data->tokens, input, AST);
		return ;
	}
	cmd = ast_to_command_invocation(data->ast, data->env_head);
	if (!cmd)
	{
		put_error_and_free(data->ast, data->tokens, input, CMD);
		return ;
	}
	run_and_cleanup(data, cmd);
}
