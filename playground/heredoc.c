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
			close(pipe_fd[0]);
			write(STDOUT_FILENO, "\n", 1);
			return (-1);
		}
	}
	return (pipe_fd[0]);
}

int	preprocess_heredocs(t_command_invocation *cmd_list)
{
	t_command_invocation	*current_cmd;
	t_cmd_redirection		*redir;

	current_cmd = cmd_list;
	while (current_cmd)
	{
		redir = current_cmd->redirections;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
			{
				redir->fd = handle_heredoc(redir->file_path);
				if (redir->fd == -1)
					return (-1);
			}
			redir = redir->next;
		}
		current_cmd = current_cmd->piped_command;
	}
	return (0);
}
