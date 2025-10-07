NAME = envlist.a
SRCS = make_env_list.c env_utils.c ft_qsort.c convert_envlist.c shlvl.c
OBJS = $(SRCS:.c=.o)

LIBFT = ../minilibft
LIBFT-A = $(LIBFT)/minilibft.a

all: $(NAME)

$(NAME): $(LIBFT_A) $(OBJS)
	ar rcs $(NAME) $(OBJS)

%.o: %.c
	cc -Wall -Wextra -Werror -I$(LIBFT) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(OBJS)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
