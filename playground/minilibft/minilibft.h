#ifndef MINILIBFT_H
# define MINILIBFT_H

# include <unistd.h>
# include <stdlib.h>
# include <limits.h>

int		ft_isalnum(int c);
int		ft_isalpha(int c);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	ft_putstr_fd(char *s, int fd);
char	*ft_strchr(const char *s, int c);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *s);
char	*ft_strdup(const char *s1);
char	*ft_strndup(const char *s1, size_t n);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	**ft_split(char *str, char c);

#endif
