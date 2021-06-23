#include "philosophers.h"

long	ft_atoi_long(char const *s)
{
	int		i;
	int		n;
	long	res;

	i = 0;
	res = 0;
	n = 1;
	while ((s[i] >= 9 && s[i] <= 13) || s[i] == 32)
		i++;
	if (s[i] == '+' || s[i] == '-')
	{
		if (s[i] == '-')
			n = n * -1;
		i++;
	}
	while (s[i] >= 48 && s[i] <= 57)
	{
		res = res * 10 + (s[i] - 48);
		i++;
	}
	return (res * n);
}