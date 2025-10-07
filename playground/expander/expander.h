/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 15:26:00 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/07 15:26:01 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include "../ast/ast.h"
# include "../envlist/envlist.h"

char	*expand_token_value(char *value, t_token_type type, t_env *env_list);
char	*expand_variables(char *str, t_env *env_list);

int		get_var_name_length(char *str);
char	*get_env_value(char *var_name, int name_len, t_env *env_list);
int		calculate_expanded_length(char *str, t_env *env_list);
#endif
