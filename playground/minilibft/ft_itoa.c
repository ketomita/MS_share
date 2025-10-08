/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ketomita <ketomita@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 10:01:37 by ketomita          #+#    #+#             */
/*   Updated: 2025/10/08 13:45:33 by ketomita         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minilibft.h"
#include <stdlib.h>
#include <limits.h>

static int	ft_checkdigits(int n)
{
	int	count;

	count = 1;
	while (n >= 10)
	{
		n /= 10;
		count++;
	}
	return (count);
}

static void	ft_convert(char *p, int n, int digits, int minus_check)
{
	int	i;

	i = digits - 1;
	p[digits + minus_check] = '\0';
	while (i >= 0)
	{
		p[i + minus_check] = n % 10 + '0';
		n /= 10;
		i--;
	}
	if (minus_check)
		p[0] = '-';
}

char	*ft_itoa(int n)
{
	char	*p;
	int		digits;
	int		minus_check;

	if (n == INT_MIN)
		return (ft_strdup("-2147483648"));
	digits = 0;
	minus_check = 0;
	if (n < 0)
	{
		minus_check++;
		n = -n;
	}
	digits = ft_checkdigits(n);
	p = malloc((digits + minus_check + 1) * sizeof(char));
	if (!p)
		return (NULL);
	ft_convert(p, n, digits, minus_check);
	return (p);
}
