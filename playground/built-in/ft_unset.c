#include "builtin.h"

static t_env	*find_prev_node(t_env *head, t_env *target_node)
{
	t_env	*current;

	current = head;
	if (!head || head == target_node)
		return (NULL);
	while (current && current->next != target_node)
		current = current->next;
	return (current);
}

static void	free_env_node(t_env *node)
{
	if (!node)
		return ;
	free(node->name);
	free(node->value);
	free(node);
}

int	ft_unset(t_data *data, char *var)
{
	t_env	*node_to_delete;
	t_env	*prev_node;

	if (!var)
		return (0);
	node_to_delete = find_env_node(data->env_head, var);
	if (!node_to_delete)
		return (0);
	prev_node = find_prev_node(data->env_head, node_to_delete);
	if (prev_node == NULL)
		data->env_head = node_to_delete->next;
	else
		prev_node->next = node_to_delete->next;
	free_env_node(node_to_delete);
	return (0);
}
