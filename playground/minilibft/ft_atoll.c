#include "minilibft.h"

long long	ft_atoll(const char *str)
{
	long long	result;
	int			sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		if (sign == 1 && (result > LLONG_MAX / 10 || \
			(result == LLONG_MAX / 10 && (*str - '0') > LLONG_MAX % 10)))
			return (LLONG_MAX);
		else if (result > -(LLONG_MIN / 10) || \
		(result == -(LLONG_MIN / 10) && (*str - '0') > -(LLONG_MIN % 10)))
			return (LLONG_MIN);
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}
