NAME = minilibft.a
SRCS = ft_isalnum.c ft_isalpha.c ft_memmove.c ft_putstr_fd.c ft_split.c ft_strchr.c ft_strcmp.c ft_strjoin.c ft_strlen.c ft_strndup.c ft_substr.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

%.o: %.c
	cc -Wall -Wextra -Werror -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME) $(OBJS)

re: fclean all

.PHONY: all clean fclean re
