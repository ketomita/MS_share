#ifndef BUILTIN
# define BUILTIN

# include <stdlib.h>
# include "../minilibft/minilibft.h"

typedef struct	s_env
{
	t_env	*prev;
	char	*name;
	char	*value;
	t_env	*next;
}	t_env;

int		ft_echo(const char **str);
int		ft_cd(char *path);
int		ft_pwd(void);
int		ft_env(char *envp[]);
void	ft_exit(int status);

#endif
