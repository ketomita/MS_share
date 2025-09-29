#include "lexer_parser.h"
#include "execute.h"

volatile sig_atomic_t	g_status;

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_token	*tokens;
	t_ast	*ast;
	t_data	data;
	char	*full_input;

	(void)argc;
	(void)argv;
	if (init_env_list(&data, envp) == -1)
	{
		ft_putstr_fd("環境変数の初期化に失敗しました\n", STDERR_FILENO);
		return (1);
	}
	g_status = 0;
	input = NULL;
	set_signal_handler();
	set_shlvl(&data);
	while (1)
	{
		input = readline_input();
		if (input == NULL)
			break ;
		full_input = handle_multiline_input(input);
		free(input);
		input = full_input;
		if (input == NULL || ft_strlen(input) == 0)
		{
			free(input);
			continue ;
		}
		tokens = tokenize(input);
		if (!tokens)
		{
			ft_putstr_fd("Tokenization failed!\n", STDERR_FILENO);
			free(input);
			continue ;
		}
		ast = parse(tokens);
		if (!ast)
		{
			ft_putstr_fd("Parsing failed!\n", STDERR_FILENO);
			free_tokens(tokens);
			free(input);
			continue ;
		}
		t_command_invocation	*cmd;
		cmd = ast_to_command_invocation(ast, data.env_head);
		if (!cmd)
		{
			ft_putstr_fd("AST to command_invocation conversion failed!\n", STDERR_FILENO);
			free_ast(ast);
			free_tokens(tokens);
			free(input);
			continue ;
		}
		data.ast = ast;
		data.cmd = cmd;
		data.input = input;
		data.tokens = tokens;
		g_status = execute_ast(cmd, &data);

		free_command_invocation(cmd);
		free_ast(ast);
		free_tokens(tokens);
		free(input);
	}
	free_env_list(data.env_head);
	rl_clear_history();
	return (g_status);
}
