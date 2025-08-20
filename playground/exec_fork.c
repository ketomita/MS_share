#include "execute.h"
#include "builtin.h"
#include "minilibft.h"

typedef enum	e_builtin
{
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT
}	t_builtin;

static void	free_split_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

static void	ft_puterror(char **cmd_args)
{
	ft_putstr_fd("Error: ", 2);
	ft_putstr_fd(cmd_args[0], 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
}

static int get_builtin_type(const char *cmd)
{
    if (!cmd)
        return (-1);
    if (ft_strcmp(cmd, "echo") == 0)
        return (ECHO);
    if (ft_strcmp(cmd, "cd") == 0)
        return (CD);
    if (ft_strcmp(cmd, "pwd") == 0)
        return (PWD);
    if (ft_strcmp(cmd, "export") == 0)
        return (EXPORT);
    if (ft_strcmp(cmd, "unset") == 0)
        return (UNSET);
    if (ft_strcmp(cmd, "env") == 0)
        return (ENV);
    if (ft_strcmp(cmd, "exit") == 0)
        return (EXIT);
    return (-1);
}

static int	execute_builtin(int type, char **cmd_args, char **envp)
{
	t_data	data;

	init_env_list(&data, envp);
	cmd_args++;
	if (type == ECHO)
		return (ft_echo((const char**)cmd_args));
	if (type == CD)
		return (ft_cd(&data, cmd_args[0]));
	if (type == PWD)
		return (ft_pwd());
	if (type == EXPORT)
		return (ft_export(&data, NULL));
	// if (type == UNSET)
	// 	return (ft_unset());
	if (type == ENV)
		return (ft_env(envp));
	if (type == EXIT)
		ft_exit(0);
	ft_putstr_fd("（ビルトイン実行）\n", 1);
	return (0);
}

static int	apply_redirects(t_ast *node)
{
	int		fd;
	t_ast	*redir_node;

	redir_node = node;
	while (redir_node->type == REDIRECT_OUT || redir_node->type == REDIRECT_APPEND || redir_node->type == REDIRECT_IN)
	{
		if (redir_node->type == REDIRECT_OUT)
		{
			fd = open(redir_node->right->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				perror(redir_node->right->value);
				return (-1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (redir_node->type == REDIRECT_APPEND)
		{
			fd = open(redir_node->right->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				perror(redir_node->right->value);
				return (-1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (redir_node->type == REDIRECT_IN)
		{
			fd = open(redir_node->right->value, O_RDONLY);
			if (fd == -1)
			{
				perror(redir_node->right->value);
				return (-1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		redir_node = redir_node->left;
	}
	return (0);
}

static void	exec_command_node(char **cmd_args, char **envp)
{
	char	*cmd_path;

	if (!cmd_args || !cmd_args[0])
	{
		ft_putstr_fd("Error: Command is empty.\n", 2);
		exit(127);
	}
	if (strchr(cmd_args[0], '/'))
		cmd_path = cmd_args[0];
	else
		cmd_path = ft_strjoin("/bin/", cmd_args[0]); // PATH検索のロジックは別途必要
	execve(cmd_path, cmd_args, envp);

	ft_puterror(cmd_args);
	if (cmd_path != cmd_args[0])
		free(cmd_path);
	// 子プロセスはexitするのでcmd_argsの解放は不要
	exit(EXIT_FAILURE);
}

/**
 * @brief (修正) コマンドを実行するメインロジック
 */
static int execute_command(t_ast *node, char **envp)
{
	char	**cmd_args;
	t_ast	*cmd_node;
	int		builtin_type;
	pid_t	pid;
	int		status;
	int stdin_backup, stdout_backup, result;


	cmd_node = node;
	while (cmd_node->type == REDIRECT_OUT || cmd_node->type == REDIRECT_APPEND || cmd_node->type == REDIRECT_IN)
		cmd_node = cmd_node->left;
	cmd_args = ft_split(cmd_node->value, ' ');
	if (!cmd_args || !cmd_args[0])
	{
		if (cmd_args)
			free_split_array(cmd_args);
		return (0); // 空コマンドは成功とする
	}
	builtin_type = get_builtin_type(cmd_args[0]);

	// === ビルトインコマンドの場合 ===
	if (builtin_type != -1)
	{
		// リダイレクトに備え、標準入出力をバックアップ
		stdin_backup = dup(STDIN_FILENO);
		stdout_backup = dup(STDOUT_FILENO);

		if (apply_redirects(node) == -1)
		{
			// エラー時はFDを復元
			dup2(stdin_backup, STDIN_FILENO);
			dup2(stdout_backup, STDOUT_FILENO);
			close(stdin_backup);
			close(stdout_backup);
			free_split_array(cmd_args);
			return (EXIT_FAILURE);
		}

		// ビルトインを実行
		result = execute_builtin(builtin_type, cmd_args, envp);

		// バックアップから標準入出力を復元
		dup2(stdin_backup, STDIN_FILENO);
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdin_backup);
		close(stdout_backup);

		free_split_array(cmd_args);
		return (result);
	}
	// === 外部コマンドの場合 ===
	else
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			free_split_array(cmd_args);
			return (1);
		}
		if (pid == 0) // 子プロセス
		{
			if (apply_redirects(node) == -1)
				exit(EXIT_FAILURE);
			exec_command_node(cmd_args, envp); // 修正後の関数を呼び出し
		}
		else // 親プロセス
		{
			free_split_array(cmd_args); // 親では不要なので解放
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				return (WEXITSTATUS(status));
			return (1); // 異常終了
		}
	}
	return (0);
}

int	execute_ast(t_ast *node, char **envp)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status1;
	int		status2;

	if (!node)
		return (0);
	if (node->type == PIPE)
	{
		if (pipe(pipefd) == -1)
		{
			perror("pipe");
			return (1);
		}
		pid1 = fork();
		if (pid1 == -1)
		{
			perror("fork");
			return (1);
		}
		if (pid1 == 0) // 子プロセス1 (パイプの左側)
		{
			close(pipefd[0]);
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[1]);
			exit(execute_ast(node->left, envp)); // 再帰呼び出し
		}
		pid2 = fork();
		if (pid2 == -1)
		{
			perror("fork");
			return (1);
		}
		if (pid2 == 0) // 子プロセス2 (パイプの右側)
		{
			close(pipefd[1]);
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			exit(execute_ast(node->right, envp)); // 再帰呼び出し
		}
		// 親プロセス (シェル本体)
		close(pipefd[0]);
		close(pipefd[1]);
		waitpid(pid1, &status1, 0);
		waitpid(pid2, &status2, 0);
		// パイプライン全体の終了ステータスは、最後のコマンドのものを返すのが一般的
		if (WIFEXITED(status2))
			return (WEXITSTATUS(status2));
		return (1);
	}
	else
		return (execute_command(node, envp));
}
