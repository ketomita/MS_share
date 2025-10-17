/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 10:27:54 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/14 15:19:41 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>

static int	is_empty_or_whitespace(const char *str)
{
	if (!str)
		return (1);
	while (*str)
	{
		if (*str != ' ' && *str != '\t' && *str != '\n' && \
			*str != '\v' && *str != '\f' && *str != '\r')
			return (0);
		str++;
	}
	return (1);
}

static int	is_dot_only(const char *str, int exit_status)
{
	if (!ft_strcmp(str, "."))
	{
		ft_putstr_fd("minishell: .: filename argument required\n", \
			STDERR_FILENO);
		ft_putstr_fd(".: usage: . filename [arguments]\n", \
			STDERR_FILENO);
		exit_status = BUILTIN_ERROR_STATUS;
		return (1);
	}
	(void)exit_status;
	return (0);
}

static void	main_roop(t_data *data)
{
	char	*input;
	char	*full_input;

	while (1)
	{
		input = readline_input();
		if (input == NULL)
			break ;
		if (is_empty_or_whitespace(input) || \
			is_dot_only(input, data->exit_status))
		{
			free(input);
			continue ;
		}
		full_input = handle_multiline_input(input);
		free(input);
		parse_and_execute(full_input, data);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void)argc;
	(void)argv;
	if (init_env_list(&data, envp) == -1)
	{
		ft_putstr_fd("環境変数の初期化に失敗しました\n", STDERR_FILENO);
		return (1);
	}
	set_signal_handler();
	set_shlvl(&data);
	main_roop(&data);
	free_env_list(data.env_head);
	rl_clear_history();
	return (data.exit_status);
}
