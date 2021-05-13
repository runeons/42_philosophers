/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsantoni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/11 15:06:27 by tsantoni          #+#    #+#             */
/*   Updated: 2019/12/18 09:44:47 by tsantoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libftprintf.h>

char	ft_find_conv(t_data *current, const char *fmt)
{
	char	*all_conv;
	int		i;
	int		j;

	all_conv = "cspdiuxX%";
	i = current->fmt_i + 1;
	while (fmt[i])
	{
		j = 0;
		while (all_conv[j])
			if (fmt[i] == all_conv[j])
			{
				current->conv_i = i;
				return (all_conv[j]);
			}
			else
				j++;
		i++;
	}
	current->conv_i = i;
	return ('\0');
}

void	ft_parse_align(t_data *current, const char *fmt)
{
	int	i;

	i = current->fmt_i + 1;
	while (fmt[i] != current->conv)
	{
		if (fmt[i] == '-')
			current->align = 1;
		i++;
	}
}

void	ft_parse_zero(t_data *current, const char *fmt)
{
	int	i;

	i = current->fmt_i + 1;
	while (fmt[i] != current->conv)
	{
		if (fmt[i] == '0' && ft_is_digit(fmt[i - 1]) == 0 && fmt[i - 1] != '.')
			current->zero = 1;
		i++;
	}
}

void	ft_parse_prec(t_data *current, const char *fmt, va_list av)
{
	int	i;
	int	buf_arg;

	i = current->fmt_i;
	while (fmt[++i] != current->conv)
		if (fmt[i] == '.')
		{
			if (ft_is_digit(fmt[i + 1]) == 0)
				if (ft_is_token('*', fmt[i + 1]) == 1)
				{
					if ((buf_arg = va_arg(av, int)) < 0)
						current->prec = -1;
					else
						current->prec = buf_arg;
				}
				else
					current->prec = 0;
			else
			{
				current->prec = 0;
				while (ft_is_digit(fmt[i + 1]) == 1)
					current->prec = current->prec * 10 + (fmt[i++ + 1] - '0');
			}
		}
}

void	ft_parse_width(t_data *current, const char *fmt, va_list av)
{
	int	i;
	int buf_arg;

	i = current->fmt_i;
	while (fmt[++i] != current->conv)
	{
		if (ft_is_digit(fmt[i]) == 1 && fmt[i - 1] != '.'
				&& ft_is_digit(fmt[i - 1]) == 0)
			if (!(fmt[i] == '0' && ft_is_digit(fmt[i + 1]) != 1))
			{
				current->width = fmt[i] - '0';
				while (ft_is_digit(fmt[i + 1]) == 1)
					current->width = current->width * 10 + (fmt[i++ + 1] - '0');
			}
		if (ft_is_token('*', fmt[i]) == 1 && fmt[i - 1] != '.')
		{
			buf_arg = va_arg(av, int);
			current->width = buf_arg;
			if (buf_arg < 0)
			{
				current->align = 1;
				current->width = -buf_arg;
			}
		}
	}
}
