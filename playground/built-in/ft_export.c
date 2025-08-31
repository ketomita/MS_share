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

static int	put_error_and_free(char *name, char *value, char *var)
{
	if (var)
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(var, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
	}
	free(name);
	free(value);
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
	eq_ptr = ft_strchr(var, '=');
	if (eq_ptr)
	{
		name = ft_substr(var, 0, eq_ptr - var);
		value = ft_strdup(eq_ptr + 1);
	}
	else
	{
		name = ft_strdup(var);
		value = NULL;
	}
	if (!name || (eq_ptr && !value))
		return (put_error_and_free(name, value, NULL));
	if (!is_valid_identifier(name))
		return (put_error_and_free(name, value, var));
	return (update_or_add_env(data, name, value));
}
