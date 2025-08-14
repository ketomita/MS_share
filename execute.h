#ifndef EXECUTE_H
#define EXECUTE_H

#include <stddef.h>

size_t	ft_strlen(const char *str);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	ft_putstr_fd(char *s, int fd);
char	**ft_split(char *str, char c);
char	*ft_strjoin(char const *s1, char const *s2);

void	set_signal_handler(void);
char	*readline_input(void);

int	execute_part(char *str[], char *envp[], int count);

#endif
