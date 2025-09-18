#include "lexer_parser.h"

t_cmd_redirection	*create_redirection(t_redirect_type type, const char *file_path)
{
	t_cmd_redirection	*redir;

	redir = malloc(sizeof(t_cmd_redirection));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->fd = -1;
	redir->next = NULL;
	if (file_path)
	{
		redir->file_path = malloc(ft_strlen(file_path) + 1);
		if (!redir->file_path)
		{
			free(redir);
			return (NULL);
		}
		ft_strlcpy(redir->file_path, file_path, ft_strlen(file_path) + 1);
	}
	else
		redir->file_path = NULL;
	return (redir);
}

void	add_redirection(t_cmd_redirection **head, t_cmd_redirection *new_redir)
{
	t_cmd_redirection	*current;

	if (!head || !new_redir)
		return ;
	if (!*head)
	{
		*head = new_redir;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_redir;
}

void	free_redirections(t_cmd_redirection *head)
{
	t_cmd_redirection	*current;
	t_cmd_redirection	*next;

	current = head;
	while (current)
	{
		next = current->next;
		free(current->file_path);
		free(current);
		current = next;
	}
}

void	free_command_invocation(t_command_invocation *cmd)
{
	int	i;

	if (!cmd)
		return ;
	free_redirections(cmd->redirections);
	if (cmd->exec_and_args)
	{
		i = 0;
		while (cmd->exec_and_args[i])
		{
			free((char *)cmd->exec_and_args[i]);
			i++;
		}
		free(cmd->exec_and_args);
	}
	free_command_invocation(cmd->piped_command);
	free(cmd);
}

static t_redirect_type	token_to_redirect_type(t_token_type type)
{
	if (type == REDIRECT_IN)
		return (REDIR_INPUT);
	else if (type == REDIRECT_OUT)
		return (REDIR_OUTPUT);
	else if (type == REDIRECT_APPEND)
		return (REDIR_APPEND);
	else if (type == REDIRECT_HEREDOC)
		return (REDIR_HEREDOC);
	return (REDIR_INPUT);
}

static int	count_args(t_ast *node)
{
	int	count;

	count = 0;
	while (node)
	{
		if (node->type == EXPANDABLE || node->type == EXPANDABLE_QUOTED || \
			node->type == NON_EXPANDABLE)
			count++;
		node = node->right;
	}
	return (count);
}

static const char	**create_args_array(t_ast *node, t_env *env_list)
{
	const char	**args;
	int			count;
	int			i;
	char		*expanded_value;

	count = count_args(node);
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	i = 0;
	while (node && i < count)
	{
		if (node->type == EXPANDABLE || node->type == EXPANDABLE_QUOTED || \
			node->type == NON_EXPANDABLE)
		{
			expanded_value = expand_token_value(node->value, node->type, env_list);
			if (!expanded_value)
			{
				while (--i >= 0)
					free((char *)args[i]);
				free(args);
				return (NULL);
			}
			args[i] = expanded_value;
			i++;
		}
		node = node->right;
	}
	args[i] = NULL;
	return (args);
}

static void	process_redirections(t_ast *node, t_command_invocation *cmd, t_env *env_list)
{
	t_cmd_redirection	*redir;
	t_redirect_type		redir_type;
	char				*expanded_path;

	while (node)
	{
		if (is_redirect_token(node->type))
		{
			redir_type = token_to_redirect_type(node->type);
			if (node->left && node->left->value)
			{
				expanded_path = expand_token_value(node->left->value, node->left->type, env_list);
				if (expanded_path)
				{
					redir = create_redirection(redir_type, expanded_path);
					free(expanded_path);
					if (redir)
						add_redirection(&cmd->redirections, redir);
				}
			}
		}
		node = node->right;
	}
}

static t_command_invocation	*convert_simple_command(t_ast *ast, t_env *env_list)
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
	cmd->exec_and_args = create_args_array(ast->left, env_list);
	process_redirections(ast->right, cmd, env_list);
	if (!cmd->exec_and_args || !cmd->exec_and_args[0])
	{
		if (!cmd->redirections)
			free_command_invocation(cmd);
		return (NULL);
	}
	return (cmd);
}

// 修正箇所　連続パイプの対応
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

t_command_invocation	*ast_to_command_invocation(t_ast *ast, t_env *env_list)
{
	t_command_invocation	*cmd;
	t_command_invocation	*piped_cmd;
	t_command_invocation	*last_cmd;

	if (!ast)
		return (NULL);

	if (ast->type == PIPE)
	{
		cmd = ast_to_command_invocation(ast->left, env_list);
		if (!cmd)
			return (NULL);
		piped_cmd = ast_to_command_invocation(ast->right, env_list);
		if (!piped_cmd)
		{
			free_command_invocation(cmd);
			return (NULL);
		}
		last_cmd = find_last_command(cmd);
		last_cmd->piped_command = piped_cmd;
		return (cmd);
	}
	else if (ast->type == NODE_COMMAND)
		return (convert_simple_command(ast, env_list));
	return (NULL);
}

// デバック
/*
static const char	*redirect_type_to_string(t_redirect_type type)
{
	if (type == REDIR_INPUT)
		return ("入力リダイレクト (<)");
	else if (type == REDIR_OUTPUT)
		return ("出力リダイレクト (>)");
	else if (type == REDIR_APPEND)
		return ("追記リダイレクト (>>)");
	else if (type == REDIR_HEREDOC)
		return ("ヒアドキュメント (<<)");
	return ("不明");
}

static void	print_indent(int level)
{
	int	i;

	i = 0;
	while (i < level)
	{
		printf("  ");
		i++;
	}
}

static void	print_redirection_details(t_cmd_redirection *redir, int level, const char *title)
{
	print_indent(level);
	printf("%s:\n", title);
	while (redir)
	{
		print_indent(level + 1);
		printf("• %s → %s\n",
			redirect_type_to_string(redir->type),
			redir->file_path ? redir->file_path : "(null)");
		redir = redir->next;
	}
}

// command_invocationの内容を表示 デバッグ用
void	print_command_invocation(t_command_invocation *cmd, int level)
{
	int	i;

	if (!cmd)
		return ;

	print_indent(level);
	printf("┌─ コマンド %d ─────────────────\n", level + 1);

	// 実行ファイルと引数を表示
	if (cmd->exec_and_args)
	{
		print_indent(level);
		printf("│ 実行コマンド: ");
		i = 0;
		while (cmd->exec_and_args[i])
		{
			if (i == 0)
				printf("'%s'", cmd->exec_and_args[i]);
			else
				printf(" '%s'", cmd->exec_and_args[i]);
			i++;
		}
		printf("\n");
	}

	// 入力リダイレクションを表示
	if (cmd->input_redirections)
	{
		print_indent(level);
		printf("│\n");
		print_redirection_details(cmd->input_redirections, level, "│ 入力リダイレクション");
	}

	// 出力リダイレクションを表示
	if (cmd->output_redirections)
	{
		print_indent(level);
		printf("│\n");
		print_redirection_details(cmd->output_redirections, level, "│ 出力リダイレクション");
	}

	print_indent(level);
	printf("└────────────────────────────\n");

	// パイプされたコマンドを表示
	if (cmd->piped_command)
	{
		print_indent(level);
		printf("    ↓ パイプ (|)\n");
		print_command_invocation(cmd->piped_command, level);
	}
}
*/
