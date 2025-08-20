#include "builtin.h"

static int	print_env_list(t_env *head)
{
	while (head)
	{
		if (head->value)
			printf("declare -x %s=\"%s\"\n", head->name, head->value);
		else
			printf("declare -x %s\n", head->name);
		head = head->next;
	}
	return (0);
}

static t_env	*find_env_node(t_env *head, const char *name)
{
	while (head)
	{
		if (ft_strcmp(head->name, name) == 0)
			return (head);
		head = head->next;
	}
	return (NULL);
}

static int	is_valid_identifier(const char *name, char *var, char *value)
{
	if (!name || (!ft_isalpha(*name) && *name != '_'))
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(var, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		free((char *)name);
		free(value);
		return (0);
	}
	name++;
	while (*name)
	{
		if (!ft_isalnum(*name) && *name != '_')
		{
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(var, 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			free((char *)name);
			free(value);
			return (0);
		}
		name++;
	}
	return (1);
}

static int	update_or_add_env(t_data *data, char *name, char *value)
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
			return (1);
		}
		append_env_node(&data->env_head, &data->env_tail, node);
	}
	return (0);
}

int	ft_export(t_data *data, char *var)
{
	char	*name;
	char	*value;
	char	*eq_ptr;

	if (!var)
		return (print_env_list(data->env_head));
	name = NULL;
	value = NULL;
	eq_ptr = ft_strchr(var, '=');
	if (eq_ptr)
	{
		name = ft_substr(var, 0, eq_ptr - var);
		value = ft_strdup(eq_ptr + 1);
	}
	else
		name = ft_strdup(var);
	if (!name || (eq_ptr && !value))
	{
		free(name);
		free(value);
		return (1);
	}
	if (!is_valid_identifier(name, var, value))
		return (1);
	return (update_or_add_env(data, name, value));
}
