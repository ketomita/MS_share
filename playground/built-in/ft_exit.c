#include "builtin.h"

static bool	is_numeric_string(const char *str)
{
	if (!str || !*str)
		return (false);
	if (*str == '+' || *str == '-')
		str++;
	if (!*str)
		return (false);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (false);
		str++;
	}
	return (true);
}

static void	free_all_resources(t_data *data)
{
	free_env_list(data->env_head);
	free_command_invocation(data->cmd);
	free_ast(data->ast);
	free_tokens(data->tokens);
	free(data->input);
	rl_clear_history();
}

static void	put_exit_error(t_data *data, char *str, int exit_status)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	free_all_resources(data);
	exit(exit_status);
}

static void	correct_args_exit(t_data *data, char **args, int arg_count)
{
	long long	status;

	if (arg_count == 1)
	{
		free_all_resources(data);
		exit(g_status);
		exit (0);
	}
	if (arg_count == 2)
	{
		if (is_numeric_string(args[1]))
		{
			status = ft_atoll(args[1]);
			free_all_resources(data);
			exit((unsigned char)status);
		}
		else
			put_exit_error(data, args[1], 2);
	}
}

int	ft_exit(t_data *data, char **args)
{
	int			arg_count;

	ft_putstr_fd("exit\n", STDOUT_FILENO);
	arg_count = count_args(args);
	correct_args_exit(data, args, arg_count);
	if (arg_count > 2)
	{
		if (!is_numeric_string(args[1]))
			put_exit_error(data, args[1], 255);
		else
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", \
				STDERR_FILENO);
			return (1);
		}
	}
	return (0);
}
