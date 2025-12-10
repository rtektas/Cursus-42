/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 18:30:31 by asaracut          #+#    #+#             */
/*   Updated: 2025/10/24 18:33:27 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

int	ft_arg_count(const char	*str)
{
	unsigned int	i;

	i = 0;
	while (*str)
	{
		if (*str == '%')
		{
			i++;
		}
		str++;
	}
	return (i);
}

int	ft_dispatch(char c, va_list args)
{
	int	nb_count;

	nb_count = 0;
	if (c == 'c')
		nb_count += ft_putchar(va_arg(args, int));
	else if (c == 's')
		nb_count += ft_putstr(va_arg(args, char *));
	else if (c == 'p')
		nb_count += ft_aff_point(va_arg(args, long long int));
	else if (c == 'd')
		nb_count += ft_putnbr(va_arg(args, int));
	else if (c == 'i')
		nb_count += ft_putnbr(va_arg(args, int));
	else if (c == 'u')
		nb_count += ft_putnbr_unsigned(va_arg(args, unsigned int));
	else if (c == 'x' || c == 'X')
		nb_count += ft_putnbr_hexa(va_arg(args, unsigned int), c);
	else if (c == '%')
		nb_count += ft_putchar('%');
	return (nb_count);
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		print_char;
	int		i;

	i = 0;
	print_char = 0;
	va_start(args, str);
	while (str[i])
	{
		if (str[i] == '%')
			print_char += ft_dispatch(str[++i], args);
		else
			print_char += ft_putchar(str[i]);
		i++;
	}
	va_end(args);
	return (print_char);
}
