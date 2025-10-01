#ifndef ENVLIST_H
# define ENVLIST_H

# include <stdlib.h>
# include "../minilibft/minilibft.h"
# include <readline/readline.h>

# include "../lexer_parser.h"

void	free_env_list(t_env *head);
t_env	*make_env_node(char *name, char *value);
void	append_env_node(t_env **head, t_env **tail, t_env *node);
t_env	*find_env_node(t_env *head, const char *name);

int		init_env_list(t_data *data, char *envp[]);

void	ft_qsort(t_env **arr, int low, int high);

char	**convert_env_list_to_array(t_env *head);

t_env	**envcpy_and_get_size(t_env *head, size_t *size);

#endif
