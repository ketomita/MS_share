NAME = minishell

CC = cc
FLAGS = -Wall -Wextra -Werror

LIBS = -lreadline

LIBFT = ./minilibft
LIBFT-A = $(LIBFT)/minilibft.a

BUILT-IN = ./built-in
BUILT-IN_A = $(BUILT-IN)/built-in.a

ENVLIST = ./envlist
ENVLIST_A = $(ENVLIST)/envlist.a

SRCS = input.c execute.c lexer.c main.c parser.c command_execution.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(BUILT-IN_A):
	make -C $(BUILT-IN)

$(LIBFT-A):
	make -C $(LIBFT)

$(ENVLIST_A):
	make -C $(ENVLIST)

%.o: %.c
	$(CC) $(FLAGS) -I$(BUILT-IN) -I$(LIBFT) -I$(ENVLIST_A) -c $< -o $@

$(NAME): $(BUILT-IN_A) $(LIBFT-A) $(ENVLIST_A) $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME) $(LIBS) $(BUILT-IN_A) $(LIBFT-A) $(ENVLIST_A)

clean:
	rm -f $(OBJS)
	make -C $(BUILT-IN) clean
	make -C $(LIBFT) clean
	make -C $(ENVLIST) clean

fclean:
	rm -f $(OBJS)
	rm -f $(NAME)
	make -C $(BUILT-IN) fclean
	make -C $(LIBFT) fclean
	make -C $(ENVLIST) fclean

re: fclean all

.PHONY: all clean fclean re
