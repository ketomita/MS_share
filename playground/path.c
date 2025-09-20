#include "execute.h"

static void	free_split(char **str)
{
	size_t	i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
}

static char	*prepare_cmd_path(char *cmd, char **paths)
{
	size_t	i;
	char	*tmp;
	char	*full_path;

	i = 0;
	if (!paths)
		return (NULL);
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		tmp = full_path;
		full_path = ft_strjoin(full_path, cmd);
		free(tmp);
		if (access(full_path, X_OK) == 0)
		{
			free_split(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_split(paths);
	return (NULL);
}
char	*find_command_path(char *cmd, t_env *env_list)
{
	char	**paths;
	char	*path_env_value;
	t_env	*path_node;

	if (!cmd || !*cmd)
		return (NULL);
	path_node = find_env_node(env_list, "PATH");
	if (path_node == NULL)
		path_env_value = NULL;
	else
		path_env_value = path_node->value;
	if (path_env_value == NULL)
		paths = NULL;
	else
	{
		paths = ft_split(path_env_value, ':');
		if (!paths)
			return (NULL);
	}
	return (prepare_cmd_path(cmd, paths));
}
