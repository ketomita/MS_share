#include "execute.h"

static void	restore_fds(int stdin_backup, int stdout_backup)
{
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
}

int	execute_builtin(t_command_invocation *cmd, t_data *data)
{
	int	stdin_backup;
	int	stdout_backup;
	int	result;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (apply_redirections(cmd))
	{
		restore_fds(stdin_backup, stdout_backup);
		return (1);
	}
	result = dispatch_builtin((char **)cmd->exec_and_args, data);
	restore_fds(stdin_backup, stdout_backup);
	return (result);
}
