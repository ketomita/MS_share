#include "easter.h"

int  is_nyancat(const char *str)
{
	int		fd;
	ssize_t	bytes_read;
	char	buf[42];

	if (ft_strcmp(str, "nyancat") != 0)
		return (0);
	fd = open("./nyancat", O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("nyancat: ./nyancat: No such file or directory\n", 2);
		return (1);
	}
	while ((bytes_read = read(fd, buf, 42)) > 0)
	{
		if (write(STDOUT_FILENO, buf, bytes_read) != bytes_read)
		{
			ft_putstr_fd("nyancat: write error\n", 2);
			close(fd);
			return (1);
		}
	}
	close(fd);
	if (bytes_read == -1)
		ft_putstr_fd("nyancat: read error\n", 2);
	return (1);
}