#include "envlist.h"

static t_env	*split_env(char *envp)
{
	char	*eq;
	char	*name;
	char	*value;
	t_env	*node;

	eq = ft_strchr(envp, '=');
	if (!eq)
		return (NULL);
	name = ft_strndup(envp, eq - envp);
	value = ft_strdup(eq + 1);
	if (!name || !value)
	{
		free(name);
		free(value);
		return (NULL);
	}
	node = make_env_node(name, value);
	if (!node)
	{
		free(name);
		free(value);
	}
	return (node);
}

int	init_env_list(t_data *data, char *envp[])
{
	t_env	*node;
	size_t	i;

	data->env_head = NULL;
	data->env_tail = NULL;
	i = 0;
	while (envp[i])
	{
		node = split_env(envp[i]);
		if (!node)
		{
			free_env_list(data->env_head);
			return (-1);
		}
		append_env_node(&data->env_head, &data->env_tail, node);
		i++;
	}
	return (0);
}
