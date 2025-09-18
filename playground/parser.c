#include "lexer_parser.h"

t_ast	*create_ast_node(t_token_type type, const char *value)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	if (value)
	{
		node->value = malloc(ft_strlen(value) + 1);
		if (!node->value)
		{
			free(node);
			return (NULL);
		}
		ft_strcpy(node->value, value);
	}
	else
		node->value = NULL;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

void	free_ast(t_ast *node)
{
	if (!node)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	free(node->value);
	free(node);
}

bool	is_redirect_token(t_token_type type)
{
	return (type == REDIRECT_IN || type == REDIRECT_OUT
		|| type == REDIRECT_APPEND || type == REDIRECT_HEREDOC);
}

static t_token	*advance_token(t_parser *parser)
{
	if (parser->current && parser->current->type != EOF_TOKEN)
		parser->current = parser->current->next;
	return (parser->current);
}

static t_ast	*parse_redirection(t_parser *parser)
{
	t_ast	*redirect_node;
	t_ast	*file_node;

	if (!is_redirect_token(parser->current->type))
		return (NULL);
	redirect_node = create_ast_node(parser->current->type,
			parser->current->value);
	if (!redirect_node)
		return (NULL);
	advance_token(parser);
	if (parser->current->type != EXPANDABLE
		&& parser->current->type != EXPANDABLE_QUOTED \
		&& parser->current->type != NON_EXPANDABLE)
	{
		free_ast(redirect_node);
		return (NULL);
	}
	file_node = create_ast_node(parser->current->type, parser->current->value);
	if (!file_node)
	{
		free_ast(redirect_node);
		return (NULL);
	}
	redirect_node->left = file_node;
	advance_token(parser);
	return (redirect_node);
}

static void	append_node(t_ast **list_head, t_ast **list_tail, t_ast *new_node)
{
	if (*list_head == NULL)
	{
		*list_head = new_node;
		*list_tail = new_node;
	}
	else
	{
		(*list_tail)->right = new_node;
		*list_tail = new_node;
	}
}

static t_ast	*parse_simple_command(t_parser *parser)
{
	t_ast	*cmd_node;
	t_ast	*arg_list_head = NULL;
	t_ast	*redirect_list_head = NULL;
	t_ast	*last_arg = NULL;
	t_ast	*last_redirect = NULL;
	t_ast	*new_node;

	cmd_node = create_ast_node(NODE_COMMAND, NULL);
	if (!cmd_node)
		return (NULL);

	while (parser->current && parser->current->type != EOF_TOKEN && parser->current->type != PIPE)
	{
		if (is_redirect_token(parser->current->type))
		{
			new_node = parse_redirection(parser);
			if (!new_node)
			{
				free_ast(arg_list_head);
				free_ast(cmd_node);
				return (NULL);
			}
			append_node(&redirect_list_head, &last_redirect, new_node);
		}
		else if (parser->current->type == EXPANDABLE || \
				parser->current->type == EXPANDABLE_QUOTED || \
				parser->current->type == NON_EXPANDABLE)
		{
			new_node = create_ast_node(parser->current->type, parser->current->value);
			if (!new_node)
			{
				free_ast(arg_list_head);
				free_ast(redirect_list_head);
				free_ast(cmd_node);
				return (NULL);
			}
			append_node(&arg_list_head, &last_arg, new_node);
			advance_token(parser);
		}
		else
			break;
	}
	if (!arg_list_head && !redirect_list_head)
	{
		free_ast(cmd_node);
		return (NULL);
	}
	cmd_node->left = arg_list_head;
	cmd_node->right = redirect_list_head;
	return (cmd_node);
}

static t_ast	*parse_pipeline(t_parser *parser)
{
	t_ast	*left;
	t_ast	*pipe_node;
	t_ast	*right;

	left = parse_simple_command(parser);
	if (!left)
		return (NULL);
	while (parser->current && parser->current->type == PIPE)
	{
		pipe_node = create_ast_node(PIPE, "|");
		if (!pipe_node)
		{
			free_ast(left);
			return (NULL);
		}
		advance_token(parser);
		right = parse_simple_command(parser);
		if (!right)
		{
			free_ast(left);
			free_ast(pipe_node);
			return (NULL);
		}
		pipe_node->left = left;
		pipe_node->right = right;
		left = pipe_node;
	}
	return (left);
}

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

void	print_ast(t_ast *node, int level)
{
	const char *type_names[] = {"EXPANDABLE", "EXPANDABLE_QUOTED",
		"NON_EXPANDABLE", "PIPE", "REDIRECT_IN", "REDIRECT_OUT",
		"REDIRECT_APPEND", "REDIRECT_HEREDOC", "NODE_COMMAND", "EOF_TOKEN"};
	int i;

	if (!node)
		return ;
	i = 0;
	while (i < level)
	{
		printf("  ");
		i++;
	}
	if (node->value)
		printf("%s: %s\n", type_names[node->type], node->value);
	else
		printf("%s: %s\n", type_names[node->type], "(null)");
	if (node->left)
	{
		i = 0;
		while (i < level)
		{
			printf("  ");
			i++;
		}
		printf("├─ left:\n");
		print_ast(node->left, level + 1);
	}
	if (node->right)
	{
		i = 0;
		while (i < level)
		{
			printf("  ");
			i++;
		}
		printf("└─ right:\n");
		print_ast(node->right, level + 1);
	}
}
