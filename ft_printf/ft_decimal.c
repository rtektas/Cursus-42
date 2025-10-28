/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_decimal.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 12:05:57 by rtektas           #+#    #+#             */
/*   Updated: 2024/05/03 16:46:48 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_char(char *s, unsigned int number, int len)
{
	int	index;

	index = len;
	while (number > 0)
	{
		s[index] = '0' + (number % 10);
		number /= 10;
		index--;
	}
	return (s);
}

int	ft_len(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n != 0)
	{
		len++;
		n = n / 10;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char			*s;
	int				len;
	unsigned int	nombre;
	int				sign;

	sign = 1;
	len = ft_len(n);
	s = (char *)malloc(sizeof(char) * (len + 1));
	if (!(s))
		return (0);
	s[len--] = '\0';
	if (n == 0)
		s[0] = '0';
	if (n < 0)
	{
		sign *= -1;
		nombre = n * -1;
		s[0] = '-';
	}
	else
		nombre = n;
	s = ft_char(s, nombre, len);
	return (s);
}

int	ft_putnbr(int n)
{
	int		len;
	char	*num;

	num = ft_itoa(n);
	len = ft_putstr(num);
	free(num);
	return (len);
}
