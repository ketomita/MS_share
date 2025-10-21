/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 14:04:48 by hhayato           #+#    #+#             */
/*   Updated: 2025/10/21 15:26:58 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include <stdlib.h>

static int	is_valid_arg_type(t_token_type type)
{
	return (type == EXPANDABLE || type == EXPANDABLE_QUOTED
		|| type == NON_EXPANDABLE);
}

static void	add_split_args_to_list(t_list **arg_list, char **split_args)
{
	int		i;
	t_list	*new_node;

	if (!split_args)
		return ;
	i = 0;
	while (split_args[i])
	{
		new_node = ft_lstnew(split_args[i]);
		if (!new_node)
		{
			ft_lstclear(arg_list, free);
			free_string_array(split_args);
			return ;
		}
		ft_lstadd_back(arg_list, new_node);
		i++;
	}
	free(split_args);
}

static bool	add_to_list(t_ast *node, t_list **arg_list, char *expanded_value)
{
	t_list	*new_node;
	char	**split_args;

	split_args = NULL;
	if (node->type == EXPANDABLE)
	{
		split_args = ft_split(expanded_value, ' ');
		free(expanded_value);
		if (!split_args)
			return (ft_lstclear(arg_list, free), false);
		add_split_args_to_list(arg_list, split_args);
	}
	else
	{
		new_node = ft_lstnew(expanded_value);
		if (!new_node)
		{
			free(expanded_value);
			ft_lstclear(arg_list, free);
			return (false);
		}
		ft_lstadd_back(arg_list, new_node);
	}
	return (true);
}

static char	**convert_list_to_array(t_list *arg_list)
{
	char	**args;
	int		count;
	int		i;
	t_list	*current;

	count = ft_lstsize(arg_list);
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
	{
		ft_lstclear(&arg_list, free);
		return (NULL);
	}
	i = 0;
	current = arg_list;
	while (current)
	{
		args[i] = (char *)current->content;
		current = current->next;
		i++;
	}
	args[i] = NULL;
	ft_lstclear(&arg_list, NULL);
	return (args);
}

char	**create_args_array(t_ast *node, t_data *data)
{
	t_list	*arg_list;
	char	*expanded_value;

	arg_list = NULL;
	while (node)
	{
		if (is_valid_arg_type(node->type))
		{
			expanded_value = expand_token_value(node->value, node->type, data);
			if (!expanded_value)
			{
				ft_lstclear(&arg_list, free);
				return (NULL);
			}
			if (!add_to_list(node, &arg_list, expanded_value))
				return (NULL);
		}
		node = node->right;
	}
	return (convert_list_to_array(arg_list));
}
