/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_calc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 14:05:32 by hhayato           #+#    #+#             */
/*   Updated: 2025/10/08 13:56:34 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include <stdlib.h>

static int	calc_status_len(int exit_status)
{
	char	*status_str;
	int		len;

	status_str = ft_itoa(exit_status);
	if (!status_str)
		return (0);
	len = ft_strlen(status_str);
	free(status_str);
	return (len);
}

static int	calc_env_var_len(char *str, int *i, t_env *env_list)
{
	int		var_name_len;
	char	*env_value;
	int		len;

	var_name_len = get_var_name_length(&str[*i + 1]);
	env_value = get_env_value(&str[*i + 1], var_name_len, env_list);
	len = 0;
	if (env_value)
	{
		len = ft_strlen(env_value);
		free(env_value);
	}
	*i += var_name_len + 1;
	return (len);
}

int	calculate_expanded_length(char *str, t_env *env_list, t_data *data)
{
	int	total_len;
	int	i;

	total_len = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] == '?')
		{
			total_len += calc_status_len(data->exit_status);
			i += 2;
		}
		else if (str[i] == '$' && str[i + 1] && \
			(ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
			total_len += calc_env_var_len(str, &i, env_list);
		else
		{
			total_len++;
			i++;
		}
	}
	return (total_len);
}
