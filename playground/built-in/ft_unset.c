/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 10:07:21 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/07 10:07:22 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

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

	if (!var)
		return (0);
	node_to_delete = find_env_node(data->env_head, var);
	if (!node_to_delete)
		return (0);
	if (node_to_delete->prev)
		node_to_delete->prev->next = node_to_delete->next;
	else
		data->env_head = node_to_delete->next;
	if (node_to_delete->next)
		node_to_delete->next->prev = node_to_delete->prev;
	else
		data->env_tail = node_to_delete->prev;
	free_env_node(node_to_delete);
	return (0);
}
