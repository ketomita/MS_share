int	ft_exit(int status)
{
	if (status >= 256)
		exit(status % 256);
	exit(status);
}
