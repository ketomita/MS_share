#include <unistd.h>

#include "built-in.h"

int	ft_env(char *envp[])
{
	int	i;

	if (!envp)
		return (1);
	i = 0;
	while (envp[i] != NULL)
	{
		write(1, envp[i], ft_strlen(envp[i]));
		write(1, "\n", 1);
		i++;
	}
	return (0);
}

// int main(int argc, char *argv[], char *envp[])
// {
// 	(void)argc;
// 	(void)argv;
// 	return (ft_env(envp));
// }
