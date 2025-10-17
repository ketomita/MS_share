/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 10:07:14 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/08 13:52:38 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static int	print_env_list(t_env *head)
{
	int		size;
	t_env	**env_array;
	int		i;

	env_array = envcpy_and_get_size(head, &size);
	if (!env_array)
		return (0);
	if (size > 1)
		ft_qsort(env_array, 0, size - 1);
	i = 0;
	while (i < size)
	{
		if (env_array[i]->value)
			printf("declare -x %s=\"%s\"\n", \
				env_array[i]->name, env_array[i]->value);
		else
			printf("declare -x %s\n", env_array[i]->name);
		i++;
	}
	free(env_array);
	return (0);
}

static int	is_valid_identifier(const char *name)
{
	const char	*p;

	p = name;
	if (!p || (!ft_isalpha(*p) && *p != '_'))
		return (0);
	p++;
	while (*p)
	{
		if (!ft_isalnum(*p) && *p != '_')
			return (0);
		p++;
	}
	return (1);
}

static void	put_error_and_free(char *name, char *value, char *var, int *exit_status)
{
	if (var)
	{
		ft_putstr_fd("export: `", STDERR_FILENO);
		ft_putstr_fd(var, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	}
	free(name);
	free(value);
	*exit_status = 1;
}

static void	update_or_add_env(t_data *data, char *name, char *value, int *exit_status)
{
	t_env	*node;

	node = find_env_node(data->env_head, name);
	if (node)
	{
		free(name);
		if (value)
		{
			free(node->value);
			node->value = value;
		}
	}
	else
	{
		node = make_env_node(name, value);
		if (!node)
		{
			free(name);
			free(value);
			*exit_status = 1;
		}
		append_env_node(&data->env_head, &data->env_tail, node);
	}
}

int	ft_export(t_data *data, char **args)
{
	int		i;
	char	*name;
	char	*value;
	char	*eq_ptr;
	int		exit_status;

	exit_status = 0;
	if (args[1] == NULL)
		return (print_env_list(data->env_head));
	i = 1;
	while (args[i])
	{
		eq_ptr = ft_strchr(args[i], '=');
		if (eq_ptr)
		{
			name = ft_substr(args[i], 0, eq_ptr - args[i]);
			value = ft_strdup(eq_ptr + 1);
		}
		else
		{
			name = ft_strdup(args[i]);
			value = NULL;
		}
		if (!name || (eq_ptr && !value))
			put_error_and_free(name, value, NULL, &exit_status);
		else if (!is_valid_identifier(name))
			put_error_and_free(name, value, args[i], &exit_status);
		update_or_add_env(data, name, value, &exit_status);
		i++;
	}
	return (exit_status);
}
