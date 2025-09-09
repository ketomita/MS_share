#include "execute.h"

char	*readline_input(void)
{
	char	*line;

	line = readline("minishell$ ");
	if (line == NULL)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		return (NULL);
	}
	if (ft_strlen(line) > 0)
		add_history(line);
	return (line);
}
