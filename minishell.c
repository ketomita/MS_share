#include "execute.h"
#include <stddef.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(int argc, char *argv[], char *envp[])
{
	char	*line;
	char	**commands;
	int		count;
	int		i;

	(void)argv;
	if (argc != 1)
		return (1);
	while (1)
	{
		line = readline_input();
		if (line == NULL)
			break ;
		commands = ft_split(line, ' ');
		free(line);
		count = 0;
		while (commands[count] != NULL)
			count++;
		execute_part(commands, envp, count);
		i = 0;
		while (i < count)
			free(commands[i++]);
		free(commands);
	}
	rl_clear_history();
	return (0);
}
