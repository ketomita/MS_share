#ifndef ENVLIST_H
# define ENVLIST_H

# include <stdlib.h>
# include "../minilibft/minilibft.h"

typedef struct s_env
{
	struct s_env	*prev;
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_data
{
	t_env	*env_head;
	t_env	*env_tail;
}	t_data;

void	free_env_list(t_env *head);
t_env	*make_env_node(char *name, char *value);
void	append_env_node(t_env **head, t_env **tail, t_env *node);
t_env	*find_env_node(t_env *head, const char *name);

int		init_env_list(t_data *data, char *envp[]);
#endif
