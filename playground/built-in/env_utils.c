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
