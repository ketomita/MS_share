/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 10:07:21 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/08 13:52:57 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include <stdlib.h>

static void	free_env_node(t_env *node)
{
	if (!node)
		return ;
	free(node->name);
	free(node->value);
	free(node);
}

static int 	is_valid_identifier(const char *name)
{
	if (!name || (!ft_isalpha(*name) && *name != '_'))
		return (0);
	name++;
	while (*name)
	{
		if (!ft_isalnum(*name) && *name != '_')
			return (0);
		name++;
	}
	return (1);
}

int	ft_unset(t_data *data, char **args)
{
	int		i;
	t_env	*node_to_delete;
	int		exit_status;

	if (args[1] == NULL)
		return (0);
	exit_status = 0;
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("unset: `", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			exit_status = 1;
		}
		else
		{
			node_to_delete = find_env_node(data->env_head, args[i]);
			if (node_to_delete)
			{
				if (node_to_delete->prev)
					node_to_delete->prev->next = node_to_delete->next;
				else
					data->env_head = node_to_delete->next;
				if (node_to_delete->next)
					node_to_delete->next->prev = node_to_delete->prev;
				else
					data->env_tail = node_to_delete->prev;
				free_env_node(node_to_delete);
			}
		}
		i++;
	}
	return (exit_status);
}
