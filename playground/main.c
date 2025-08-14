#include "lexer_parser.h"
#include "execute.h"

int	main(int argc, char *argv[], char *envp[])
{
	char	*input;
	t_token	*tokens;
	t_ast	*ast;

	(void)argv;
	if (argc != 1)
		return (1);
	while (1)
	{
		input = readline_input();
		if (input == NULL)
			break ;
		if (input[strlen(input) - 1] == '\n')
		input[strlen(input) - 1] = '\0';
		
		if (strlen(input) == 0)
		{
			printf("Enter a command (or press Ctrl+D to exit):\n");
			continue;
		}
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
			execute_ast(ast, envp);
			free_ast(ast);
		}
		free_tokens(tokens);
		printf("\nEnter a command (or press Ctrl+D to exit):\n");
	}
	free(input);
	rl_clear_history();
	return (0);
}