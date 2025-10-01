#include "envlist.h"

void	free_env_list(t_env *head)
{
	t_env	*tmp;

	while (head)
	{
		tmp = head->next;
		free(head->name);
		free(head->value);
		free(head);
		head = tmp;
	}
}

t_env	*make_env_node(char *name, char *value)
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

void	append_env_node(t_env **head, t_env **tail, t_env *node)
{
	if (!*head)
		*head = node;
	else
	{
		(*tail)->next = node;
		node->prev = *tail;
	}
	*tail = node;
}

t_env	*find_env_node(t_env *head, const char *name)
{
	while (head)
	{
		if (ft_strcmp(head->name, name) == 0)
			return (head);
		head = head->next;
	}
	return (NULL);
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
