/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_env_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 10:18:42 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/07 11:51:57 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	int		i;

	data->env_head = NULL;
	data->env_tail = NULL;
	data->stdin_backup = -1;
	data->stdout_backup = -1;
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
