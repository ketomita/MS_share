#include "envlist.h"

static size_t	count_env_nodes(t_env *head)
{
	size_t	count;

	count = 0;
	while (head)
	{
		count++;
		head = head->next;
	}
	return (count);
}

char	**convert_env_list_to_array(t_env *head)
{
	char	**env_array;
	size_t	i;
	size_t	node_count;
	char	*temp;

	node_count = count_env_nodes(head);
	env_array = (char **)malloc(sizeof(char *) * (node_count + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	while (head)
	{
		temp = ft_strjoin(head->name, "=");
		env_array[i] = ft_strjoin(temp, head->value);
		free(temp);
		if (!env_array[i])
		{
			while (i > 0)
				free(env_array[--i]);
			free(env_array);
			return (NULL);
		}
		head = head->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}
