#ifndef MINILIBFT_H
# define MINILIBFT_H

# include <unistd.h>
# include <stdlib.h>
# include <limits.h>

void	*ft_memmove(void *dest, const void *src, size_t n);
void	ft_putstr_fd(char *s, int fd);
char	**ft_split(char *str, char c);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *s);

#endif
