#ifndef EXECUTE_H
#define EXECUTE_H

#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <limits.h>

#include "lexer_parser.h"

size_t	ft_strlen(const char *str);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	ft_putstr_fd(char *s, int fd);
char	**ft_split(char *str, char c);
char	*ft_strjoin(char const *s1, char const *s2);

void	set_signal_handler(void);
char	*readline_input(void);

int	execute_ast(t_ast *node, char **envp);

#endif
