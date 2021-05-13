/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_libft.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsantoni <tsantoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 15:04:46 by tsantoni          #+#    #+#             */
/*   Updated: 2021/03/01 13:56:39 by tharchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libftprintf.h>

void	ft_puts(t_data *current, char *str)
{
	int	i;

	i = 0;
	while (str[i])
		ft_putchar(current, str[i++]);
}

int		ft_strlen_pf(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int		ft_is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

char	*ft_strdup_pf(char *str)
{
	char	*res;
	int		i;
	int		len;

	i = 0;
	len = ft_strlen_pf(str);
	if (!(res = malloc(sizeof(char) * (len + 1))))
		return (NULL);
	while (i < len)
	{
		res[i] = str[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

char	*ft_strndup_pf(char *str, int n)
{
	char	*res;
	int		i;
	int		len;

	i = 0;
	len = ft_strlen_pf(str);
	if (len > n)
		len = n;
	if (!(res = malloc(sizeof(char) * (len + 1))))
		return (NULL);
	while (i < len)
	{
		res[i] = str[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}
