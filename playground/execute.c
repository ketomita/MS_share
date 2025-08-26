#include "execute.h"

void	free_split(char **str)
{
	size_t	i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
}

bool	is_builtin(const char *cmd)
{
	if (cmd == NULL)
		return (false);
	if (ft_strcmp(cmd, "echo") == 0)
		return (true);
	if (ft_strcmp(cmd, "cd") == 0)
		return (true);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (true);
	if (ft_strcmp(cmd, "export") == 0)
		return (true);
	if (ft_strcmp(cmd, "unset") == 0)
		return (true);
	if (ft_strcmp(cmd, "env") == 0)
		return (true);
	if (ft_strcmp(cmd, "exit") == 0)
		return (true);
	return (false);
}

bool	is_special_builtin(const char *cmd)
{
	if (!cmd)
		return (false);
	if (ft_strcmp(cmd, "cd") == 0)
		return (true);
	if (ft_strcmp(cmd, "export") == 0)
		return (true);
	if (ft_strcmp(cmd, "unset") == 0)
		return (true);
	if (ft_strcmp(cmd, "exit") == 0)
		return (true);
	return (false);
}

int	dispatch_builtin(char **args, t_data data)
{
	const char	*cmd;

	cmd = args[0];
	if (ft_strcmp(cmd, "echo") == 0)
		return (ft_echo((const char **)(args + 1)));
	if (ft_strcmp(cmd, "cd") == 0)
		return (ft_cd(&data, args[1]));
	if (ft_strcmp(cmd, "pwd") == 0)
		return (ft_pwd());
	if (ft_strcmp(cmd, "export") == 0)
		return (ft_export(&data, args[1]));
	if (ft_strcmp(cmd, "unset") == 0)
		return (ft_unset(&data, args[1]));
	if (ft_strcmp(cmd, "env") == 0)
		return (ft_env(data.env_head));
	if (ft_strcmp(cmd, "exit") == 0)
		return (ft_exit(&data, args));
	return (1);
}

/**
 * @brief 標準入出力を復元する
 */
static void	restore_fds(int stdin_backup, int stdout_backup)
{
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
}

static int	prepro_cmd_path(char **envp, char ***paths)
{
	size_t	i;
	char	*path_env;

	path_env = NULL;
	i = 0;
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_env = envp[i] + 5;
			break ;
		}
		i++;
	}
	if (!path_env)
	{
		*paths = NULL;
		return (0);
	}
	*paths = ft_split(path_env, ':');
	if (!*paths)
		return (1);
	return (0);
}

static char	*prepare_cmd_path(char *cmd, char **paths)
{
	size_t	i;
	char	*tmp;
	char	*full_path;

	i = 0;
	if (!paths)
		return (NULL);
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		tmp = full_path;
		full_path = ft_strjoin(full_path, cmd);
		free(tmp);
		if (access(full_path, X_OK) == 0)
		{
			free_split(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_split(paths);
	return (NULL);
}

static char	*find_command_path(char *cmd, char **envp)
{
	char	**paths;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	if (prepro_cmd_path(envp, &paths))
		return (NULL);
	return (prepare_cmd_path(cmd, paths));
}

/**
 * @brief リダイレクトを適用する
 */
static int	apply_redirections(t_command_invocation *cmd)
{
	t_cmd_redirection	*redir;
	int					fd;

	redir = cmd->input_redirections;
	while (redir)
	{
		fd = open(redir->file_path, O_RDONLY);
		if (fd == -1)
		{
			perror(redir->file_path);
			return (-1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
		redir = redir->next;
	}
	redir = cmd->output_redirections;
	while (redir)
	{
		if (redir->type == REDIR_OUTPUT)
			fd = open(redir->file_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			fd = open(redir->file_path, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			perror(redir->file_path);
			return (-1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
		redir = redir->next;
	}
	return (0);
}

/**
 * @brief （親プロセスで実行する）ビルトインコマンドの処理
 */
static int	execute_builtin(t_command_invocation *cmd, t_data data)
{
	int	stdin_backup;
	int	stdout_backup;
	int	result;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (apply_redirections(cmd) == -1)
	{
		restore_fds(stdin_backup, stdout_backup);
		return (1);
	}
	result = dispatch_builtin((char **)cmd->exec_and_args, data);
	restore_fds(stdin_backup, stdout_backup);
	return (result);
}

static void ft_put_execve_error(char *str)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": ", 2);
	perror(NULL);
	exit(127);
}

/**
 * @brief 子プロセス内で行う処理（入出力の設定とコマンド実行）
 */
static void	execute_child_process(t_command_invocation *cmd, char **envp, int in_fd, int out_fd, t_data data)
{
	char	*path;

	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (out_fd != STDOUT_FILENO)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	if (apply_redirections(cmd) == -1)
		exit(1);
	if (is_builtin(cmd->exec_and_args[0]))
		exit(dispatch_builtin((char **)cmd->exec_and_args, data));
	else
	{
		path = find_command_path((char *)cmd->exec_and_args[0], envp);
		if (path)
		{
			execve(path, (char **)cmd->exec_and_args, envp);
			free(path);
		}
		ft_put_execve_error((char *)cmd->exec_and_args[0]);
	}
}

/**
 * @brief 単一コマンドを適切な方法（親or子プロセス）で実行する
 * @param in_fd このコマンドが読み込むべき入力ファイルディスクリプタ
 * @param out_fd このコマンドが書き込むべき出力ファイルディスクリプタ
 * @return pid_t 子プロセスのPID、またはビルトイン用のステータス
 */
static pid_t	execute_simple_command(t_command_invocation *cmd, char **envp, int in_fd, int out_fd, t_data data)
{
	pid_t	pid;

	if (!cmd || !cmd->exec_and_args || !cmd->exec_and_args[0])
		return (0);

	// パイプがなく、かつcd, export, unset, exitのような親プロセスで実行すべきビルトインの場合
	if (in_fd == STDIN_FILENO && out_fd == STDOUT_FILENO && is_special_builtin(cmd->exec_and_args[0]))
	{
		return (execute_builtin(cmd, data));
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0) // 子プロセス
	{
		execute_child_process(cmd, envp, in_fd, out_fd, data);
	}

	return (pid);
}

/**
 * @brief パイプライン全体を処理する
 */
static int	execute_pipeline(t_command_invocation *cmd_list, char **envp, t_data data)
{
	int						status;
	int						pipe_fd[2];
	int						in_fd;
	pid_t					last_pid;
	t_command_invocation	*current_cmd;

	status = 0;
	in_fd = STDIN_FILENO; // 最初のコマンドの入力は標準入力
	last_pid = -1;
	current_cmd = cmd_list;
	while (current_cmd)
	{
		// 次のコマンドがある場合（パイプが必要な場合）
		if (current_cmd->piped_command)
		{
			if (pipe(pipe_fd) == -1)
			{
				perror("pipe");
				return (1); // エラー
			}
			// コマンドを実行（入力はin_fd, 出力はpipe_fd[1]）
			last_pid = execute_simple_command(current_cmd, envp, in_fd, pipe_fd[1], data);
			close(pipe_fd[1]); // 子プロセスが複製したので親は不要
			if (in_fd != STDIN_FILENO)
				close(in_fd); // 前のパイプの読み取り口を閉じる
			in_fd = pipe_fd[0]; // 次のコマンドの入力は今回のパイプの読み取り口
		}
		else // パイプラインの最後のコマンド
		{
			// コマンドを実行（入力はin_fd, 出力は標準出力）
			last_pid = execute_simple_command(current_cmd, envp, in_fd, STDOUT_FILENO, data);
			if (in_fd != STDIN_FILENO)
				close(in_fd);
		}
		current_cmd = current_cmd->piped_command;
	}
	// 最後の子プロセスの終了を待ち、そのステータスを取得
	if (last_pid > 0)
	{
		waitpid(last_pid, &status, 0);
		// 他のすべての子プロセスを待つ（ゾンビプロセス防止）
		while (wait(NULL) != -1 || errno != ECHILD)
			;
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (status); // シグナル終了などの場合
}

/**
 * @brief (メインの実行関数) コマンドリストを受け取り実行を開始する
 *
 * @param cmd_list ast_to_command_invocationで生成されたコマンドリストの先頭
 * @param envp 環境変数
 * @return int 最後のコマンドの終了ステータス
 */
int	execute_ast(t_command_invocation *cmd_list, char **envp, t_data data)
{
	if (!cmd_list)
		return (0);
	return (execute_pipeline(cmd_list, envp, data));
}
