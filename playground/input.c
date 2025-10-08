/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 09:59:12 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/08 14:03:44 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

char	*readline_input(void)
{
	char	*line;

	line = readline("minishell$ ");
	if (line == NULL)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		return (NULL);
	}
	if (ft_strlen(line) > 0)
		add_history(line);
	return (line);
}
