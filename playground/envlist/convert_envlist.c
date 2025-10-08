/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_envlist.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 10:11:00 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/08 13:54:15 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "envlist.h"
#include <stdlib.h>

static int	count_env_nodes(t_env *head)
{
	int	count;

	count = 0;
	while (head)
	{
		if (head->value)
			count++;
		head = head->next;
	}
	return (count);
}

static int	free_env_array(char **env_array, int i)
{
	while (i > 0)
		free(env_array[--i]);
	free(env_array);
	return (1);
}

static int	convert_env_nodes(t_env *head, char **env_array, int *i)
{
	char	*temp;

	if (head->value)
	{
		temp = ft_strjoin(head->name, "=");
		if (!temp)
			return (free_env_array(env_array, 0));
		env_array[*i] = ft_strjoin(temp, head->value);
		free(temp);
		if (!env_array[*i])
			return (free_env_array(env_array, *i));
		(*i)++;
	}
	return (0);
}

char	**convert_env_list_to_array(t_env *head)
{
	char	**env_array;
	int		i;
	int		node_count;

	node_count = count_env_nodes(head);
	env_array = (char **)malloc(sizeof(char *) * (node_count + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	while (head)
	{
		if (convert_env_nodes(head, env_array, &i))
			return (NULL);
		head = head->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
