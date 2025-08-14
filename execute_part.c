#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "execute.h"

static void	free_split_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

static void	child_process_handler(int input_fd, int output_fd, char *cmd_str, char **envp)
{
	char	**cmd_args;
	char	*cmd_path;

	// 標準入力(0)を入力元fdに、標準出力(1)を出力先fdに差し替える (リダイレクト)
	if (dup2(input_fd, STDIN_FILENO) == -1)
	{
		ft_putstr_fd(strerror(errno), 2);
		exit(EXIT_FAILURE);
	}
	if (dup2(output_fd, STDOUT_FILENO) == -1)
	{
		ft_putstr_fd(strerror(errno), 2);
		exit(EXIT_FAILURE);
	}
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	if (output_fd != STDOUT_FILENO)
		close(output_fd);
	cmd_args = ft_split(cmd_str, ' ');
	if (!cmd_args || !cmd_args[0])
	{
		ft_putstr_fd("Error: Command is empty.\n", 2);
		exit(127);
	}
	if (strchr(cmd_args[0], '/'))
		cmd_path = cmd_args[0];
	else
		cmd_path = ft_strjoin("/bin/", cmd_args[0]);
	execve(cmd_path, cmd_args, envp);
	ft_putstr_fd("Error: ", 2);
	ft_putstr_fd(cmd_args[0], 2);
	free(cmd_path);
	free_split_array(cmd_args);
	exit(EXIT_FAILURE);
}

int	execute_part(char *str[], char *envp[], int count)
{
	int	pid[count];
	int	i;
	int	pipe_fd[count - 1][2];
	int	status;

	i = 0;
	while (i < count - 1)
	{
		if (pipe(pipe_fd[i++]) < 0)
			ft_putstr_fd(strerror(errno), 2);
	}
	i = 0;
	while (i < count)
	{
		pid[i] = fork();
		if (pid[i] < 0)
		{
			ft_putstr_fd(strerror(errno), 2);
			return (-1);
		}
		if (pid[i] == 0) // ========= 子プロセスの処理 =========
		{
			int	input_fd;
			int	output_fd;
			int	j;

			// 入力元を決定する
			if (i == 0)
				input_fd = STDIN_FILENO; // 最初のコマンドは標準入力
			else
				input_fd = pipe_fd[i - 1][0]; // 2番目以降は前のパイプの読み込み口

			// 出力先を決定する
			if (i == count - 1)
				output_fd = STDOUT_FILENO; // 最後のコマンドは標準出力
			else
				output_fd = pipe_fd[i][1]; // 途中は次のパイプの書き込み口

			// このプロセスが使わないパイプディスクリプタをすべて閉じる
			j = 0;
			while (j < count - 1)
			{
				// 自分が入力に使う読み込み口でなければ閉じる
				if (i > 0 && j == i - 1)
					close(pipe_fd[j][1]); // 書き込み口だけ閉じる
				// 自分が出力に使う書き込み口でなければ閉じる
				else if (i < count - 1 && j == i)
					close(pipe_fd[j][0]); // 読み込み口だけ閉じる
				// それ以外のパイプは両方閉じる
				else
				{
					close(pipe_fd[j][0]);
					close(pipe_fd[j][1]);
				}
				j++;
			}
			// コマンド実行ハンドラを呼び出す
			child_process_handler(input_fd, output_fd, str[i], envp);
		}
		i++;
	}
	i = 0;
	while (i < count - 1)
	{
		close(pipe_fd[i][0]);
		close(pipe_fd[i][1]);
		i++;
	}
	i = 0;
	while (i < count)
		waitpid(pid[i++], &status, 0);
	return (0);
}
