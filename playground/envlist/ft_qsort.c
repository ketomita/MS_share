#include "envlist.h"

static void	swap_nodes(t_env **a, t_env **b)
{
	t_env	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

static int	partition(t_env **arr, int low, int high)
{
	char	*pivot_name;
	int		i;
	int		j;

	pivot_name = arr[high]->name;
	i = low - 1;
	j = low;
	while (j < high)
	{
		if (ft_strcmp(arr[j]->name, pivot_name) <= 0)
		{
			i++;
			swap_nodes(&arr[i], &arr[j]);
		}
		j++;
	}
	swap_nodes(&arr[i + 1], &arr[high]);
	return (i + 1);
}

void	ft_qsort(t_env **arr, int low, int high)
{
	int	pi;

	if (low < high)
	{
		pi = partition(arr, low, high);
		ft_qsort(arr, low, pi - 1);
		ft_qsort(arr, pi + 1, high);
	}
}
