/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 17:05:23 by rtektas           #+#    #+#             */
/*   Updated: 2024/04/19 20:04:18 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_espace(int c)
{
	if ((c > 8 && c < 14) || (c == 32))
		return (1);
	return (0);
}

static int	check_number(int c)
{
	if (c > 47 && c < 58)
		return (1);
	return (0);
}

int	ft_atoi(char const *str)
{
	long long int	n;
	long long int	check;
	int				sign;

	n = 0;
	sign = 1;
	while (*str && check_espace(*str))
		str++;
	if (*str == 45 || *str == 43)
	{
		if (*str == 45)
			sign *= -1;
		str++;
	}
	while (*str && check_number(*str))
	{
		check = n;
		n = n * 10 + sign * (*str - 48);
		if (n > check && sign < 0)
			return (0);
		if (n < check && sign > 0)
			return (-1);
		str++;
	}
	return (n);
}
