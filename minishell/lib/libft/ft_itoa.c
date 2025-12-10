/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 18:30:31 by asaracut          #+#    #+#             */
/*   Updated: 2025/10/24 18:32:12 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

static int	ft_size(int nb)
{
	int	size;

	size = 0;
	if (nb < 0)
	{
		nb = nb * (-1);
		size = 1;
	}
	if (nb == 0)
		size = 1;
	else
	{
		while (nb)
		{
			nb = nb / 10;
			size++;
		}
	}
	return (size);
}

char	*ft_itoa(int n)
{
	int		i;
	long	nb;
	char	*res;
	int		neg;

	i = ft_size(n);
	res = (char *) malloc(sizeof(char) * (i + 1));
	if (!res)
		return (NULL);
	nb = (long) n;
	neg = 0;
	if (nb < 0)
	{
		nb = nb * (-1);
		res[0] = '-';
		neg = 1;
	}
	res[i] = '\0';
	while (i > neg)
	{
		res[i - 1] = nb % 10 + '0';
		nb = nb / 10;
		i--;
	}
	return (res);
}
