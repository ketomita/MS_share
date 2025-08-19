#include "built-in.h"

static t_env	*make_env_node(char *name, char *value)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->name = name;
	node->value = value;
	node->prev = NULL;
	node->next = NULL;
	return (node);
}

static int	split_envp(char *envp[], char *name[], char *value[], int size)
{
	int		i;

	i = 0;
	while (envp[i])
	{
		name = ft_split(envp[i]);
	}
}

int	make_env_list(char *envp[], int size)
{
	int		i;
	t_env	*node;

	i = 0;
	while (i < size)
	{
		node = make_env_node();
		if (!node)
		{
			free_list();
			return (1);
		}
		i++;
	}
	return (0);
}
