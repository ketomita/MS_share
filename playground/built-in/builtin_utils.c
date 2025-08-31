#include "builtin.h"

size_t	count_args(char **args)
{
	size_t	i;

	if (!args)
		return (0);
	i = 0;
	while (args[i])
		i++;
	return (i);
}
