/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 14:05:00 by hhayato           #+#    #+#             */
/*   Updated: 2025/10/14 11:43:25 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include <stdlib.h>

static t_command_invocation	*convert_simple_command(t_data *data, t_ast *ast)
{
	t_command_invocation	*cmd;

	if (ast->type != NODE_COMMAND)
		return (NULL);
	cmd = malloc(sizeof(t_command_invocation));
	if (!cmd)
		return (NULL);
	cmd->redirections = NULL;
	cmd->piped_command = NULL;
	cmd->pid = -1;
	cmd->exec_and_args = create_args_array(ast->left, data);
	process_redirections(ast->right, cmd, data);
	if (cmd->exec_and_args && cmd->exec_and_args[0])
		return (cmd);
	if (cmd->redirections)
		return (cmd);
	free_command_invocation(cmd);
	return (NULL);
}

static t_command_invocation	*find_last_command(t_command_invocation *cmd)
{
	t_command_invocation	*current;

	if (!cmd)
		return (NULL);
	current = cmd;
	while (current->piped_command)
		current = current->piped_command;
	return (current);
}

static t_command_invocation	*handle_pipe_ast(t_ast *pipe_node, t_data *data)
{
	t_command_invocation	*left_cmds;
	t_command_invocation	*right_cmd;

	left_cmds = ast_to_command_invocation(pipe_node->left, data);
	if (!left_cmds)
		return (NULL);
	right_cmd = ast_to_command_invocation(pipe_node->right, data);
	if (!right_cmd)
	{
		free_command_invocation(left_cmds);
		return (NULL);
	}
	find_last_command(left_cmds)->piped_command = right_cmd;
	return (left_cmds);
}

t_command_invocation	*ast_to_command_invocation(t_ast *ast, t_data *data)
{
	if (!ast)
		return (NULL);
	if (ast->type == PIPE)
		return (handle_pipe_ast(ast, data));
	else if (ast->type == NODE_COMMAND)
		return (convert_simple_command(data, ast));
	return (NULL);
}
