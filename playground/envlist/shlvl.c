/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 10:19:35 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/08 13:54:33 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "envlist.h"
#include <stdlib.h>

static void	make_shlvl_node(t_data *data)
{
	char		*name_dup;
	char		*value_dup;
	t_env		*new_node;

	name_dup = ft_strdup("SHLVL");
	value_dup = ft_strdup("1");
	if (!name_dup || !value_dup)
	{
		free(name_dup);
		free(value_dup);
		return ;
	}
	new_node = make_env_node(name_dup, value_dup);
	if (!new_node)
	{
		free(name_dup);
		free(value_dup);
		return ;
	}
	append_env_node(&data->env_head, &data->env_tail, new_node);
}

static void	update_shlvl_node(t_env *node)
{
	long long	level;
	char		*new_value;

	level = ft_atoll(node->value);
	level++;
	new_value = ft_itoa(level);
	if (!new_value)
		return ;
	free(node->value);
	node->value = new_value;
}

void	set_shlvl(t_data *data)
{
	t_env		*node;

	node = find_env_node(data->env_head, "SHLVL");
	if (node && node->value)
		update_shlvl_node(node);
	else if (!node)
		make_shlvl_node(data);
}
