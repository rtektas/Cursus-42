/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unsigned.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 16:16:26 by rtektas           #+#    #+#             */
/*   Updated: 2024/05/03 16:46:42 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_unsigned_len(unsigned int nb)
{
	int	len;

	len = 0;
	while (nb != 0)
	{
		len++;
		nb = nb / 10;
	}
	return (len);
}

char	*ft_unsigned_itoa(unsigned int nb)
{
	char	*str;
	int		len;

	len = ft_unsigned_len(nb);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (0);
	str[len] = '\0';
	while (nb != 0)
	{
		str[len - 1] = nb % 10 + 48;
		nb = nb / 10;
		len--;
	}
	return (str);
}

int	ft_putunsigned(unsigned int nb)
{
	int		affiche;
	char	*str;

	affiche = 0;
	if (nb == 0)
		affiche += write(1, "0", 1);
	else
	{
		str = ft_unsigned_itoa(nb);
		affiche += ft_putstr(str);
		free(str);
	}
	return (affiche);
}
