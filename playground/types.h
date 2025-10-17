#ifndef TYPES_H
# define TYPES_H

# include <signal.h>

/* 前方宣言（構造体本体は各モジュールヘッダに置く） */
struct									s_data;
struct									s_ast;
struct									s_token;
struct									s_cmd_redirection;
struct									s_command_invocation;
struct									s_env;

/* 公開 typedef */
typedef struct s_data					t_data;
typedef struct s_ast					t_ast;
typedef struct s_token					t_token;
typedef struct s_cmd_redirection		t_cmd_redirection;
typedef struct s_command_invocation		t_command_invocation;
typedef struct s_env					t_env;

typedef const char						t_constchar;

/* 共通列挙（トークン列挙等は lexer/lexer.h に置く） */
typedef enum e_redirect_type
{
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}				t_redirect_type;

typedef enum e_proctype
{
	PARENTS,
	CHILDREN
}	t_proctype;

typedef enum e_execve_error
{
	COMMAND_NOT_FOUND,
	COMMAND_NOT_FOUND_PATH,
	IS_A_DIRECTORY,
	PERMISSION_DENIED,
	EXECVE_ERROR
}	t_execve_error;

typedef enum e_main_error
{
	TOKENS,
	AST,
	CMD
}	t_main_error;

typedef struct s_fds
{
	int	in_fd;
	int	out_fd;
	int	pipe_fd[2];
}	t_fds;

typedef struct s_child_status
{
	pid_t	pid;
	int		status;
}	t_child_status;

#endif