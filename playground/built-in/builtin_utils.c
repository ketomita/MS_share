#include "builtin.h"

size_t	count_builtin_args(char **args)
{
	size_t	i;

	if (!args)
		return (0);
	i = 0;
	while (args[i])
		i++;
	return (i);
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
