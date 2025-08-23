#include "lexer_parser.h"
#include "execute.h"

int	main(int argc, char *argv[], char *envp[])
{
	char	*input;
	t_token	*tokens;
	t_ast	*ast;
	size_t	len;

	printf("Lexer & Parser Test Program\n");
	printf("Enter a command (or press Ctrl+D to exit):\n");

	input = NULL;
	len = 0;
	(void)argv;
	if (argc != 1)
		return (1);
	set_signal_handler();
	while (1)
	{
		input = readline_input();
		if (input == NULL)
			break ;
		if (input[ft_strlen(input) - 1] == '\n')
			input[ft_strlen(input) - 1] = '\0';

		if (ft_strlen(input) == 0)
		{
			printf("Enter a command (or press Ctrl+D to exit):\n");
			continue;
		}

		printf("\n--- Input: %s ---\n", input);

		tokens = tokenize(input);
		if (!tokens)
		{
			printf("Tokenization failed!\n");
			printf("Enter a command (or press Ctrl+D to exit):\n");
			continue;
		}

		printf("\n=== TOKENS ===\n");
		print_tokens(tokens);

		ast = parse(tokens);
		if (!ast)
		{
			printf("Parsing failed!\n");
		}
		else
		{
			t_command_invocation *cmd;

			printf("\n=== AST ===\n");
			print_ast(ast, 0);

			cmd = ast_to_command_invocation(ast);
			if (!cmd)
			{
				printf("AST to command_invocation conversion failed!\n");
			}
			else
			{
				printf("\n=== 🚀 コマンド実行構造体 ===\n");
				print_command_invocation(cmd, 0);
				execute_ast(cmd, envp);
				free_command_invocation(cmd);
			}

			free_ast(ast);
		}

		free_tokens(tokens);
		printf("\nEnter a command (or press Ctrl+D to exit):\n");
	}
	free(input);
	rl_clear_history();
	printf("\nGoodbye!\n");
	return (0);
}
