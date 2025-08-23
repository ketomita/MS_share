#ifndef BUILTIN_H
# define BUILTIN_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <stdbool.h>

# include "envlist.h"

int		ft_echo(const char **str);
int		ft_cd(t_data *data, char *path);
int		ft_pwd(void);
int		ft_env(t_env *env_list);
int		ft_exit(t_data *data, char **args);
int		ft_export(t_data *data, char *var);
int		ft_unset(t_data *data, char *var);

#endif
