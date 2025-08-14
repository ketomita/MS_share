#include <unistd.h>

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

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

int main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	return (ft_env(envp));
}
