/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 23:44:45 by hiroaki           #+#    #+#             */
/*   Updated: 2022/12/06 02:56:28 by hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/utils.h"

static int	ft_isspace(int c)
{
	return (c == ' ' || ('\t' <= c && c <= '\r'));
}

static bool	is_overflow(unsigned long num, int sign)
{
	if ((sign > 0 && num > LONG_MAX) || \
		(sign < 0 && num > (LONG_MAX + 1UL)))
		return (true);
	return (false);
}

int	ft_atoi(const char *str, bool *ok)
{
	int				sign;
	unsigned long	num;

	sign = 1;
	num = 0;
	*ok = false;
	while (ft_isspace(*str))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str++ == '-')
			sign = -1;
	}
	while (ft_isdigit(*str))
	{
		num = (num * 10) + (*str++ - '0');
		if (is_overflow(num, sign))
			return (EXIT_FAILURE);
	}
	*ok = true;
	return ((int)num * sign);
}
