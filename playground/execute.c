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
		return (ft_echo(args + 1));
	if (ft_strcmp(cmd, "cd") == 0)
		return (ft_cd(&data, args));
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

void	set_parent_signal_handlers(void)
{
	struct sigaction	sa;


	if (sigemptyset(&sa.sa_mask) != 0)
	{
		perror("sigemptyset");
		exit(EXIT_FAILURE);
	}
	sa.sa_flags = 0;
	sa.sa_handler = SIG_IGN;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
}

static int	handle_heredoc(const char *delimiter)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;
	char	*line;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);

		close(pipe_fd[0]);
		while (1)
		{
			line = readline("> ");
			if (!line || ft_strcmp(line, delimiter) == 0)
			{
				free(line);
				break;
			}
			write(pipe_fd[1], line, ft_strlen(line));
			write(pipe_fd[1], "\n", 1);
			free(line);
		}
		close(pipe_fd[1]);
		exit(0);
	}

	set_parent_signal_handlers();

	close(pipe_fd[1]);
	waitpid(pid, &status, 0);

	set_signal_handler(); //シグナル設定を元に戻す

	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			// g_status = 130;
			close(pipe_fd[0]);
			write(STDOUT_FILENO, "\n", 1);
			return (-1);
		}
	}
	return (pipe_fd[0]);
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
		if (redir->type == REDIR_HEREDOC)
			fd = handle_heredoc(redir->file_path);
		else
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
	int stdin_backup;
	int stdout_backup;
	int result;

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
	perror(str);
	exit(127);
}

/**
 * @brief 子プロセス内で行う処理（入出力の設定とコマンド実行）
 */
static void	execute_child_process(t_command_invocation *cmd, char **envp, int in_fd, int out_fd, t_data data)
{
	char	*path;

	// 子プロセスではシグナルをデフォルトの動作に戻す
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);

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
 * @brief パイプライン全体を処理する
 * @note ファイルディスクリプタ管理を修正し、デッドロックを回避
 */
static int	execute_pipeline(t_command_invocation *cmd_list, char **envp, t_data data)
{
	int						status;
	int						pipe_fd[2];
	int						in_fd;
	pid_t					pid;
	t_command_invocation	*current_cmd;
	pid_t					last_pid;
	int						cmd_count;
	pid_t					*pids;
	int						i;

	// パイプがなく、かつcd, export, unset, exitのような親プロセスで実行すべきビルトインの場合
	if (cmd_list && !cmd_list->piped_command && is_special_builtin(cmd_list->exec_and_args[0]))
	{
		return (execute_builtin(cmd_list, data));
	}

	// コマンドの数を数える
	cmd_count = 0;
	current_cmd = cmd_list;
	while(current_cmd)
	{
		cmd_count++;
		current_cmd = current_cmd->piped_command;
	}
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
	{
		perror("malloc");
		return (1);
	}

	status = 0;
	in_fd = STDIN_FILENO;
	last_pid = -1;
	current_cmd = cmd_list;
	i = 0;
	while (current_cmd)
	{
		int out_fd = STDOUT_FILENO;
		if (current_cmd->piped_command)
		{
			if (pipe(pipe_fd) == -1)
			{
				perror("pipe");
				free(pids);
				return (1);
			}
			out_fd = pipe_fd[1];
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			// TODO: エラー処理
			if (out_fd != STDOUT_FILENO) close(out_fd);
			if (in_fd != STDIN_FILENO) close(in_fd);
			free(pids);
			return (1);
		}

		if (pid == 0) // 子プロセス
		{
			if (current_cmd->piped_command)
				close(pipe_fd[0]);
			execute_child_process(current_cmd, envp, in_fd, out_fd, data);
		}
		pids[i++] = pid;
		last_pid = pid;
		if (in_fd != STDIN_FILENO)
			close(in_fd);
		if (out_fd != STDOUT_FILENO)
			close(out_fd);

		if (current_cmd->piped_command)
			in_fd = pipe_fd[0];

		current_cmd = current_cmd->piped_command;
	}

	i = 0;
	while (i < cmd_count)
	{
		int waited_pid;
		int child_status;

		waited_pid = waitpid(-1, &child_status, 0);
		if (waited_pid == last_pid)
			status = child_status;
		if (waited_pid == -1)
		{
			if (errno != ECHILD)
				perror("waitpid");
			break;
		}
		i++;
	}

	free(pids);

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGPIPE)
		{
			ft_putstr_fd("Broken pipe\n", STDERR_FILENO);
		}
		else if (WTERMSIG(status) == SIGINT || WTERMSIG(status) == SIGQUIT)
		{
			// 親シェル側で改行などを表示するため、ここでは何もしないことも多い
		}
		return (128 + WTERMSIG(status));
	}
	return (status);
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
	int	status;

	if (!cmd_list)
		return (0);

	// 子プロセスを待つ間、親はシグナルを無視する設定にする
	struct sigaction sa_ign, sa_old_int, sa_old_quit;
	sa_ign.sa_handler = SIG_IGN;
	sigemptyset(&sa_ign.sa_mask);
	sa_ign.sa_flags = 0;
	sigaction(SIGINT, &sa_ign, &sa_old_int);
	sigaction(SIGQUIT, &sa_ign, &sa_old_quit);

	status = execute_pipeline(cmd_list, envp, data);

	// 親のシグナルハンドラを元に戻す
	sigaction(SIGINT, &sa_old_int, NULL);
	sigaction(SIGQUIT, &sa_old_quit, NULL);

	return (status);
}
