#include "execute.h"

static void	readline_in_heredoc(int pipe_fd[], const char *delimiter)
{
	char	*line;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	close(pipe_fd[0]);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	exit(0);
}

static int	fork_heredoc(const char *delimiter, int pipe_fd[], int *status)
{
	pid_t	pid;

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
		readline_in_heredoc(pipe_fd, delimiter);
	set_parent_signal_handlers();
	close(pipe_fd[1]);
	waitpid(pid, status, 0);
	return (0);
}

static int	handle_heredoc(const char *delimiter)
{
	int		pipe_fd[2];
	int		status;

	if (fork_heredoc(delimiter, pipe_fd, &status))
		return (-1);
	set_signal_handler();
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

static int	apply_redirections_input(t_command_invocation *cmd)
{
	int					fd;
	t_cmd_redirection	*redir;

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
	return (0);
}

static int	apply_redirections_output(t_command_invocation *cmd)
{
	int					fd;
	t_cmd_redirection	*redir;

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

int	apply_redirections(t_command_invocation *cmd)
{
	if (apply_redirections_input(cmd))
		return (-1);
	if (apply_redirections_output(cmd))
		return (-1);
	return (0);
}
