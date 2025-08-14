#include <unistd.h>

static void	ft_putstr(const char *s)
{
	size_t	len;

	if (!s)
		return ;
	len = 0;
	while (s[len] != '\0')
		len++;
	write(1, s, len);
}

static int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

static int	check_opt(const char **str)
{
	int	i;

	i = 0;
	while (str[i] && ft_strcmp(str[i], "-n") == 0)
		i++;
	return (i);
}

int	ft_echo(const char **str)
{
	int		start;
	int		i;

	start = check_opt(str);
	i = start;
	while (str[i])
	{
		ft_putstr(str[i]);
		if (str[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (start == 0)
		write(1, "\n", 1);
	return (0);
}

// int main(int argc, char *argv[])
// {
// 	(void)argc;
// 	return (ft_echo((const char **)argv + 1));
// }
