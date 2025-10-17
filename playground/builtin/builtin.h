/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 10:06:55 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/08 13:39:01 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "../types.h"
# include "../envlist/envlist.h"
# include "../lexer/lexer.h"
# include "../parser/parser.h"
# include "../ast/ast.h"

typedef enum e_builtin
{
	BUILTIN,
	BUILTIN_PARENT,
	TRANSIENT,
	NOR
}	t_builtin;

typedef enum e_error
{
	HOME,
	ARGS,
	OLDPWD,
	GETCWD,
	CHDIR
}	t_error_type;

int			ft_echo(char **str);
int			ft_cd(t_data *data, char **args);
int			ft_pwd(void);
int			ft_env(t_env *env_list);
int			ft_exit(t_data *data, char **args);
int			ft_export(t_data *data, char *var);
int			ft_unset(t_data *data, char *var);

size_t		count_builtin_args(char **args);
int			dispatch_builtin(char **args, t_data *data);
void		free_all_resources(t_data *data);
t_builtin	is_builtin(const char *cmd);

#endif
