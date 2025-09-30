#include "builtin.h"

size_t	count_builtin_args(char **args)
{
	size_t	i;

	if (!args)
		return (0);
	i = 0;
	while (args[i])
		i++;
	return (i);
}

int	dispatch_builtin(char **args, t_data *data)
{
	const char	*cmd;

	cmd = args[0];
	if (ft_strcmp(cmd, "echo") == 0)
		return (ft_echo(args + 1));
	if (ft_strcmp(cmd, "cd") == 0)
		return (ft_cd(data, args));
	if (ft_strcmp(cmd, "pwd") == 0)
		return (ft_pwd());
	if (ft_strcmp(cmd, "export") == 0)
		return (ft_export(data, args[1]));
	if (ft_strcmp(cmd, "unset") == 0)
		return (ft_unset(data, args[1]));
	if (ft_strcmp(cmd, "env") == 0)
		return (ft_env(data->env_head));
	if (ft_strcmp(cmd, "exit") == 0)
		return (ft_exit(data, args));
	return (1);
}

void	free_all_resources(t_data *data)
{
	free_env_list(data->env_head);
	free_command_invocation(data->cmd);
	free_ast(data->ast);
	free_tokens(data->tokens);
	free(data->input);
	rl_clear_history();
}

t_env	**envcpy_and_get_size(t_env *head, size_t *size)
{
	t_env	**env_array;
	t_env	*current;
	size_t	i;

	*size = 0;
	current = head;
	while (current)
	{
		(*size)++;
		current = current->next;
	}
	if (*size == 0)
		return (NULL);
	env_array = (t_env **)malloc(sizeof(t_env *) * *size);
	if (!env_array)
		return (NULL);
	i = 0;
	current = head;
	while (current)
	{
		env_array[i++] = current;
		current = current->next;
	}
	return (env_array);
}
