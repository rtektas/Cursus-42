/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 16:41:40 by rtektas           #+#    #+#             */
/*   Updated: 2024/05/21 15:23:05 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_conversion(va_list vl, const char format)
{
	int	affiche;

	affiche = 0;
	if (format == 'c')
		affiche += ft_putchar(va_arg(vl, int));
	else if (format == 's')
		affiche += ft_putstr(va_arg(vl, char *));
	else if (format == 'p')
		affiche += ft_put_pointeur_final(va_arg(vl, unsigned long long));
	else if (format == 'd' || format == 'i')
		affiche += ft_putnbr(va_arg(vl, int));
	else if (format == 'u')
		affiche += ft_putunsigned(va_arg(vl, unsigned int));
	else if (format == 'x' || format == 'X')
		affiche += ft_put_hex_final(va_arg(vl, unsigned int), format);
	else if (format == '%')
		affiche += ft_putchar('%');
	return (affiche);
}

int	ft_printf(const char *format, ...)
{
	int		i;
	va_list	vl;
	int		affiche;

	i = 0;
	affiche = 0;
	va_start(vl, format);
	while (format[i])
	{
		if (format[i] == '%')
		{
			affiche += ft_conversion(vl, format[i + 1]);
			i++;
		}
		else
			affiche += ft_putchar(format[i]);
		i++;
	}
	va_end(vl);
	return (affiche);
}
/*
#include <stdio.h>
#define PARAMS "segerg %X couueubiu\n" , 256

int	main()
{
	printf("num :%d\n", ft_printf(PARAMS));
	printf("num :%d", printf(PARAMS));
}*/