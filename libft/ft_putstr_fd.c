/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 20:27:30 by rtektas           #+#    #+#             */
/*   Updated: 2024/04/19 20:02:56 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	int		i;
	char	*str;

	if (!s)
		return ;
	i = 0;
	str = (char *)s;
	while (str[i])
	{
		write(fd, &str[i], 1);
		i++;
	}
}
