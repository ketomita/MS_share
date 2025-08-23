#include "builtin.h"

int	ft_env(t_env *env_list)
{
	t_env	*current;

	if (!env_list)
		return (1);
	current = env_list;
	while (current)
	{
		if (current->name && current->value)
		{

			ft_putstr_fd(current->name, STDOUT_FILENO);
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putstr_fd(current->value, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		current = current->next;
	}
	return (0);
}

// int main(int argc, char *argv[], char *envp[])
// {
// 	(void)argc;
// 	(void)argv;
// 	return (ft_env(envp));
// }
