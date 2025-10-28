/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pointeur.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 16:09:21 by rtektas           #+#    #+#             */
/*   Updated: 2024/05/03 16:46:14 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_len_pointeur(unsigned long long nb)
{
	int	len;

	len = 0;
	while (nb != 0)
	{
		len++;
		nb = nb / 16;
	}
	return (len);
}

void	ft_put_pointeur(unsigned long long nb)
{
	if (nb >= 16)
	{
		ft_put_pointeur(nb / 16);
		ft_put_pointeur(nb % 16);
	}
	else
	{
		if (nb <= 9)
			ft_putchar((nb + '0'));
		else
			ft_putchar((nb - 10 + 'a'));
	}
}

int	ft_put_pointeur_final(unsigned long long ptr)
{
	int	affiche;

	affiche = 0;
	affiche += write(1, "0x", 2);
	if (ptr == 0)
		affiche += write(1, "0", 1);
	else
	{
		ft_put_pointeur(ptr);
		affiche += ft_len_pointeur(ptr);
	}
	return (affiche);
}
