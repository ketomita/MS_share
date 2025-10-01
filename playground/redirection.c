#include "execute.h"

static void	apply_redirection(t_cmd_redirection *redir, \
			int *last_input_fd, int *last_output_fd)
{
	if (redir->type == REDIR_INPUT || redir->type == REDIR_HEREDOC)
	{
		if (*last_input_fd != -1)
			close(*last_input_fd);
		if (redir->type == REDIR_HEREDOC)
			*last_input_fd = redir->fd;
		else
			*last_input_fd = open(redir->file_path, O_RDONLY);
	}
	else if (redir->type == REDIR_OUTPUT || redir->type == REDIR_APPEND)
	{
		if (*last_output_fd != -1)
			close(*last_output_fd);
		if (redir->type == REDIR_OUTPUT)
			*last_output_fd = open(redir->file_path, \
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			*last_output_fd = open(redir->file_path, \
				O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
}

static void	restore_std_fd(int last_input_fd, int last_output_fd)
{
	if (last_input_fd != -1)
	{
		dup2(last_input_fd, STDIN_FILENO);
		close(last_input_fd);
	}
	if (last_output_fd != -1)
	{
		dup2(last_output_fd, STDOUT_FILENO);
		close(last_output_fd);
	}
}

int	apply_redirections(t_command_invocation *cmd)
{
	t_cmd_redirection	*redir;
	int					last_input_fd;
	int					last_output_fd;

	last_input_fd = -1;
	last_output_fd = -1;
	redir = cmd->redirections;
	while (redir)
	{
		apply_redirection(redir, &last_input_fd, &last_output_fd);
		if ((redir->type != REDIR_HEREDOC && last_input_fd == -1 && \
			(redir->type == REDIR_INPUT)) || (last_output_fd == -1 && \
			(redir->type == REDIR_OUTPUT || redir->type == REDIR_APPEND)))
		{
			perror(redir->file_path);
			if (last_input_fd != -1)
				close(last_input_fd);
			if (last_output_fd != -1)
				close(last_output_fd);
			return (-1);
		}
		redir = redir->next;
	}
	restore_std_fd(last_input_fd, last_output_fd);
	return (0);
}
