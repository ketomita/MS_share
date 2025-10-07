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

SRCS = command_args.c command_execution.c command_free.c command_process_redir.c \
		command_redirection.c execute.c execute_builtin.c execute_pipe.c \
		execute_setup.c execute_utils.c execve.c execve_utils.c expander.c expander_calc.c \
		expander_expand.c expander_utils.c heredoc.c input.c lexer.c lexer_handlers.c \
		lexer_multiline.c lexer_token.c lexer_utils.c lexer_word.c \
		lexer_word_process.c main.c parser.c parser_ast.c parser_command.c \
		parser_command_handle.c parser_pipeline.c parser_print.c parser_print_utils.c \
		parser_utils.c path.c pipe.c redirection.c shlvl.c signal.c easteregg.c main_execute.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(BUILT-IN_A):
	make -C $(BUILT-IN)

$(LIBFT-A):
	make -C $(LIBFT)

$(ENVLIST_A):
	make -C $(ENVLIST)

%.o: %.c
	$(CC) $(FLAGS) -I$(BUILT-IN) -I$(LIBFT) -I$(ENVLIST) -c $< -o $@

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
