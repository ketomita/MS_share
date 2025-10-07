NAME = minishell

CC = cc
FLAGS = -Wall -Wextra -Werror

LIBS = -lreadline

LIBFT = ./minilibft
LIBFT-A = $(LIBFT)/minilibft.a

BUILTIN = ./builtin
BUILTIN_A = $(BUILTIN)/builtin.a

ENVLIST = ./envlist
ENVLIST_A = $(ENVLIST)/envlist.a

LEXER = ./lexer
LEXER_A = $(LEXER)/lexer.a

PARSER = ./parser
PARSER_A = $(PARSER)/parser.a

AST = ./ast
AST_A = $(AST)/ast.a

EXPANDER = ./expander
EXPANDER_A = $(EXPANDER)/expander.a

SRCS = execute.c execute_builtin.c execute_pipe.c execute_setup.c execute_utils.c \
		execve.c execve_utils.c heredoc.c input.c main.c path.c pipe.c redirection.c \
		signal.c easteregg.c main_execute.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(BUILTIN_A):
	make -C $(BUILTIN)

$(LIBFT-A):
	make -C $(LIBFT)

$(ENVLIST_A):
	make -C $(ENVLIST)

$(LEXER_A):
	make -C $(LEXER)

$(PARSER_A):
	make -C $(PARSER)

$(AST_A):
	make -C $(AST)

$(EXPANDER_A):
	make -C $(EXPANDER)

%.o: %.c
	$(CC) $(FLAGS) -I$(BUILTIN) -I$(LIBFT) -I$(ENVLIST) -I $(LEXER) -I$(PARSER) -I$(AST) -I$(EXPANDER) -c $< -o $@

$(NAME): $(BUILTIN_A) $(LIBFT-A) $(ENVLIST_A) $(LEXER_A) $(PARSER_A) $(AST_A) $(EXPANDER_A) $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME) $(BUILTIN_A) $(ENVLIST_A) $(LEXER_A) $(PARSER_A) $(AST_A) $(EXPANDER_A) $(LIBFT-A) $(LIBS)

clean:
	rm -f $(OBJS)
	make -C $(BUILTIN) clean
	make -C $(LIBFT) clean
	make -C $(ENVLIST) clean
	make -C $(LEXER) clean
	make -C $(PARSER) clean
	make -C $(AST) clean
	make -C $(EXPANDER) clean

fclean:
	rm -f $(OBJS)
	rm -f $(NAME)
	make -C $(BUILTIN) fclean
	make -C $(LIBFT) fclean
	make -C $(ENVLIST) fclean
	make -C $(LEXER) fclean
	make -C $(PARSER) fclean
	make -C $(AST) fclean
	make -C $(EXPANDER) fclean

re: fclean all

.PHONY: all clean fclean re
