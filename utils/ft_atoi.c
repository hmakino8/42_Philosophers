/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 23:44:45 by hiroaki           #+#    #+#             */
/*   Updated: 2023/02/12 21:05:19 by hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/utils.h"

static int	set_errno(int e)
{
	errno = e;
	return (-1);
}

static int	ft_isspace(int c)
{
	return (c == ' ' || ('\t' <= c && c <= '\r'));
}

static bool	check_overflow(unsigned long num, int sign)
{
	if ((sign > 0 && num > INT_MAX) || \
		(sign < 0 && num > (INT_MAX + 1UL)))
		return (true);
	return (false);
}

int	ft_atoi(const char *str)
{
	int				sign;
	unsigned long	num;

	if (str == NULL)
		return (set_errno(EINVAL));
	sign = 1;
	num = 0;
	while (ft_isspace(*str))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str++ == '-')
			sign = -1;
	}
	while (ft_isdigit(*str))
	{
		num = num * 10 + (*str++ - '0');
		if (check_overflow(num, sign))
		{
			errno = ERANGE;
			return (set_errno(ERANGE));
		}
	}
	if (*str != '\0')
		return (set_errno(EINVAL));
	return ((int)num * sign);
}
