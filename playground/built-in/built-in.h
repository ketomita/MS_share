#ifndef BUILTIN
# define BUILTIN

# include <stdlib.h>

int		ft_echo(const char **str);
int 	ft_cd(char *path);
int		ft_pwd(void);
int		ft_env(char *envp[]);
void	ft_exit(int status);

size_t	ft_strlen(const char *s);

#endif
