#include "execute.h"

void	set_shlvl(t_data *data)
{

	t_env		*node;
	char		*new_value;
	long long	level;

	node = find_env_node(data->env_head, "SHLVL");
	if (node && node->value)
	{
		level = ft_atoll(node->value);
		level++;
		new_value = ft_itoa(level);
		if (!new_value)
		{
			// ft_itoaが失敗した場合のエラー処理
			return ;
		}
		free(node->value);
		node->value = new_value;
	}
	else if (!node)
	{
		node = make_env_node("SHLVL", "1");
		append_env_node(&data->env_head, &data->env_tail, node);
	}
}
