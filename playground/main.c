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
		printf("環境変数の初期化に失敗しました\n");
		return (1);
	}

	g_status = 0;

	// printf("Lexer & Parser Test Program with Variable Expansion\n");
	// printf("Enter a command (or press Ctrl+D to exit):\n");

	input = NULL;
	set_signal_handler();

	while (1)
	{
		input = readline_input();
		if (input == NULL)
			break;
		full_input = handle_multiline_input(input);
		free(input);
		input = full_input;
		if (ft_strlen(input) == 0)
		{
			// printf("Enter a command (or press Ctrl+D to exit):\n");
			free(input);
			continue;
		}

		// printf("\n--- Input: %s ---\n", input);

		tokens = tokenize(input);
		if (!tokens)
		{
			printf("Tokenization failed!\n");
			printf("Enter a command (or press Ctrl+D to exit):\n");
			continue;
		}

		// printf("\n=== TOKENS ===\n");
		// print_tokens(tokens);

		ast = parse(tokens);
		if (!ast)
		{
			printf("Parsing failed!\n");
		}
		else
		{
			t_command_invocation *cmd;

			// printf("\n=== AST ===\n");
			// print_ast(ast, 0);

			cmd = ast_to_command_invocation(ast, data.env_head);
			if (!cmd)
			{
				printf("AST to command_invocation conversion failed!\n");
			}
			else
			{
				// printf("\n=== 🚀 コマンド実行構造体 (変数展開済み) ===\n");
				// print_command_invocation(cmd, 0);
				data.ast = ast;
				data.cmd = cmd;
				data.input = input;
				data.tokens = tokens;
				g_status = execute_ast(cmd, envp, data);
				free_command_invocation(cmd);
			}

			free_ast(ast);
		}

		free_tokens(tokens);
		// printf("\nEnter a command (or press Ctrl+D to exit):\n");
		free(input);
	}
	free_env_list(data.env_head);
	rl_clear_history();
	// printf("\nGoodbye!\n");
	return (g_status);
}
