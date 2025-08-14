NAME = minishell

CC = cc
FLAGS = -Wall -Wextra -Werror

LIBS = -lreadline

SRCS = input.c execute_for_ast.c ft_split.c ft_putstr_fd.c ft_strjoin.c ft_strlen.c ft_memmove.c lexer.c main.c parser.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME) $(LIBS)

clean:
	rm -f $(OBJS)

fclean:
	rm -f $(OBJS)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
