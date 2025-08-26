#ifndef MINILIBFT_H
# define MINILIBFT_H

# include <unistd.h>
# include <stdlib.h>
# include <limits.h>

long long	ft_atoll(const char *str);
void		ft_bzero(void *s, size_t n);
void		*ft_calloc(size_t nmemb, size_t size);
int			ft_isalnum(int c);
int			ft_isalpha(int c);
int			ft_isdigit(int c);
void		*ft_memmove(void *dest, const void *src, size_t n);
void		ft_putstr_fd(char *s, int fd);
char		*ft_strchr(const char *s, int c);
int			ft_strcmp(const char *s1, const char *s2);
char		*ft_strcpy(char *dest, const char *src);
char		*ft_strncpy(char *dest, const char *src, size_t n);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
char		*ft_strjoin(char const *s1, char const *s2);
size_t		ft_strlen(const char *s);
char		*ft_strdup(const char *s1);
char		*ft_strndup(const char *s1, size_t n);
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		**ft_split(char *str, char c);

#endif
