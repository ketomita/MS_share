#include "execute.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t	i;

	if (!dest && !src)
		return (NULL);
	i = 0;
	if (dest < src)
	{
		while (i < n)
		{
			((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
			i++;
		}
	}
	else
	{
		while (0 < n)
		{
			n--;
			((unsigned char *)dest)[n] = ((unsigned char *)src)[n];
		}
	}
	return (dest);
}

// #include<stdio.h>
// #include<string.h>

// int main()
// {
//     char buf1[] = "c2s";
//     char buf2[] = "aabb";

//     printf("配列の要素を表示(コピー前)。\n");
//     printf("buf1:%s\n", buf1);
//     printf("buf2:%s\n", buf2);

//     ft_memmove(buf2, buf1, 3);

//     printf("配列の要素を表示(コピー後)。\n");
//     printf("buf1:%s\n", buf1);
//     printf("buf2:%s\n", buf2);
//     return 0;
// }
