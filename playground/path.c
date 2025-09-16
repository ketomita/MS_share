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

static int	prepro_cmd_path(char **envp, char ***paths)
{
	size_t	i;
	char	*path_env;

	path_env = NULL;
	i = 0;
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_env = envp[i] + 5;
			break ;
		}
		i++;
	}
	if (!path_env)
	{
		*paths = NULL;
		return (0);
	}
	*paths = ft_split(path_env, ':');
	if (!*paths)
		return (1);
	return (0);
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

char	*find_command_path(char *cmd, char **envp)
{
	char	**paths;

	if (!cmd || !*cmd)
		return (NULL);
	if (prepro_cmd_path(envp, &paths))
		return (NULL);
	return (prepare_cmd_path(cmd, paths));
}
