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

int	handle_heredoc(const char *delimiter)
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

int	apply_redirections_input(t_command_invocation *cmd)
{
    int                 last_fd;
    t_cmd_redirection   *redir;

    last_fd = -1;
    redir = cmd->input_redirections;
    while (redir)
    {
        if (last_fd != -1)
            close(last_fd);
        if (redir->type == REDIR_HEREDOC)
            last_fd = redir->fd;
        else
            last_fd = open(redir->file_path, O_RDONLY);
        if (last_fd == -1)
        {
            perror(redir->file_path);
            return (-1);
        }
        redir = redir->next;
    }
    if (last_fd != -1)
    {
        dup2(last_fd, STDIN_FILENO);
        close(last_fd);
    }
    return (0);
}

int	apply_redirections_output(t_command_invocation *cmd)
{
	int					last_fd;
	t_cmd_redirection	*redir;

	last_fd = -1;
	redir = cmd->output_redirections;
	while (redir)
	{
		if (last_fd != -1)
			close(last_fd);
		if (redir->type == REDIR_OUTPUT)
			last_fd = open(redir->file_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			last_fd = open(redir->file_path, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (last_fd == -1)
		{
			perror(redir->file_path);
			return (-1);
		}
		redir = redir->next;
	}
	if (last_fd != -1)
	{
		dup2(last_fd, STDOUT_FILENO);
		close(last_fd);
	}
	return (0);
}
