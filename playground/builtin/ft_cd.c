/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 10:07:00 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/08 13:50:43 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

static void	put_error_message(int *result, t_error_type type)
{
	if (type == HOME)
		ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
	else if (type == ARGS)
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
	else if (type == OLDPWD)
		ft_putstr_fd("cd: OLDPWD not set\n", STDERR_FILENO);
	else if (type == GETCWD)
		perror("cd: getcwd");
	else if (type == CHDIR)
		perror("cd: chdir");
	*result = 1;
}

static int	change_pwd_oldpwd(t_data *data, char *old_path)
{
	char	*pwd;
	char	*oldpwd;
	char	*current_path;
	char	*export_args[3];

	oldpwd = ft_strjoin("OLDPWD=", old_path);
	export_args[0] = "export";
	export_args[1] = oldpwd;
	export_args[2] = NULL;
	ft_export(data, export_args);
	free(oldpwd);
	current_path = getcwd(NULL, 0);
	if (!current_path)
	{
		perror("cd: getcwd");
		return (1);
	}
	pwd = ft_strjoin("PWD=", current_path);
	export_args[1] = pwd;
	ft_export(data, export_args);
	free(current_path);
	free(pwd);
	return (0);
}

static void	set_home_or_target_path(char *path, \
	char **target_path, int *result, int mode)
{
	char	*home_path;

	if (mode == 0)
	{
		*target_path = getenv("HOME");
		if (!(*target_path))
			put_error_message(result, HOME);
	}
	else if (mode == 1)
	{
		home_path = getenv("HOME");
		if (!home_path)
			put_error_message(result, HOME);
		else
			*target_path = ft_strjoin(home_path, path + 1);
	}
}

static char	*set_target_path(t_data *data, \
	char **args, char **target_path, int *result)
{
	t_env	*env_buf;
	char	*path;

	if (count_builtin_args(args) > 2)
	{
		put_error_message(result, ARGS);
		path = NULL;
	}
	path = args[1];
	if (path == NULL)
		set_home_or_target_path(path, target_path, result, 0);
	else if (*path == '~')
		set_home_or_target_path(path, target_path, result, 1);
	else if (ft_strcmp(path, "-") == 0)
	{
		env_buf = find_env_node(data->env_head, "OLDPWD");
		if (!env_buf)
			put_error_message(result, OLDPWD);
		else
			*target_path = env_buf->value;
	}
	else
		*target_path = path;
	return (path);
}

int	ft_cd(t_data *data, char **args)
{
	char	*old_path;
	char	*target_path;
	int		result;
	char	*path;

	result = 0;
	old_path = getcwd(NULL, 0);
	if (!old_path)
		put_error_message(&result, GETCWD);
	path = set_target_path(data, args, &target_path, &result);
	if (result == 0 && chdir(target_path) != 0)
		put_error_message(&result, CHDIR);
	if (result == 0 && change_pwd_oldpwd(data, old_path))
		result = 1;
	free(old_path);
	if (path && *path == '~')
		free(target_path);
	if (path && ft_strcmp(path, "-") == 0 && result == 0)
		ft_pwd();
	return (result);
}
