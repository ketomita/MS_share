#include "execute.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

// #include <stdio.h>

// int main(void)
// {
// 	int ret;

// 	ret = ft_strlen("42TOKYO");
// 	printf("%d", ret);
// }
