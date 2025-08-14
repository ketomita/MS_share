#include "execute.h"

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

static void	exec_command_node(t_ast *node, char **envp)
{
	char	**cmd_args;
	char	*cmd_path;

	cmd_args = ft_split(node->value, ' ');
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
	ft_puterror(cmd_args);
	if (cmd_path != cmd_args[0])
		free(cmd_path);
	free_split_array(cmd_args);
	exit(EXIT_FAILURE);
}

int	execute_ast(t_ast *node, char **envp)
{
	int		pipefd[2];
	pid_t	pid;
	int		fd;

	if (!node)
		return (0);
	if (node->type == PIPE)
	{
		if (pipe(pipefd) == -1)
		{
			perror("pipe");
			return (1);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return (1);
		}
		if (pid == 0)
		{
			// 子プロセス（左側コマンド）
			close(pipefd[0]); // 読み取り側閉じる
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[1]);
			execute_ast(node->left, envp);
			exit(0);
		}
		else
		{
			// 親プロセス（右側コマンド）
			close(pipefd[1]); // 書き込み側閉じる
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			execute_ast(node->right, envp);
			waitpid(pid, NULL, 0);
		}
	}
	else if (node->type == REDIRECT_OUT)
	{
		fd = open(node->right->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror("open");
			return (1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
		execute_ast(node->left, envp);
	}
	else if (node->type == REDIRECT_APPEND)
	{
		fd = open(node->right->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			perror("open");
			return (1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
		execute_ast(node->left, envp);
	}
	else if (node->type == REDIRECT_IN)
	{
		fd = open(node->right->value, O_RDONLY);
		if (fd == -1)
		{
			perror("open");
			return (1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
		execute_ast(node->left, envp);
	}
	else if (node->type == EXPANDABLE || node->type == EXPANDABLE_QUOTED || node->type == NON_EXPANDABLE)
		exec_command_node(node, envp);
	else
	{
		fprintf(stderr, "Unknown AST node type: %d\n", node->type);
		return (1);
	}
	return (0);
}
