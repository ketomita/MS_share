#include "builtin.h"

static bool is_numeric_string(const char *str)
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

static long long	ft_atoll(const char *str)
{
	long long	result;
	int			sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		if (sign == 1 && (result > LLONG_MAX / 10 || (result == LLONG_MAX / 10 && (*str - '0') > LLONG_MAX % 10)))
			return (LLONG_MAX);
		else if (result > -(LLONG_MIN / 10) || (result == -(LLONG_MIN / 10) && (*str - '0') > -(LLONG_MIN % 10)))
			return (LLONG_MIN);
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}

void	free_all_resources(t_data *data)
{
	free_env_list(data->env_head);
	free_command_invocation(data->cmd);
	free_ast(data->ast);
	free_tokens(data->tokens);
	free(data->input);
	rl_clear_history();
}

int ft_exit(t_data *data, char **args)
{
	int			arg_count;
	long long	status;

	ft_putstr_fd("exit\n", STDOUT_FILENO);
	arg_count = 0;
	while (args[arg_count])
		arg_count++;
	(void)data;
	if (arg_count == 1)
	{
		free_all_resources(data); // 環境変数や履歴などを全て解放
		// exit(g_last_exit_status); // グローバル変数やdata構造体で保持している最後の終了ステータス
		exit (111);
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
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(args[1], STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			free_all_resources(data);
			exit(2);
		}
	}
	if (arg_count > 2)
	{
		if (!is_numeric_string(args[1]))
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(args[1], STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			free_all_resources(data);
			exit(255);
		}
		else
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
			return (1);
		}
	}
	return (0);
}
