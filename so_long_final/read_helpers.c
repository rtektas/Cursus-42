/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 00:00:00 by rtektas           #+#    #+#             */
/*   Updated: 2025/11/14 00:00:00 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

char	*join_and_free(char *a, char *b)
{
	char	*res;

	if (!a)
		return (ft_strdup(b));
	res = ft_strjoin(a, b);
	free(a);
	return (res);
}

int	count_coins(char **rows)
{
	int	i;
	int	j;
	int	cnt;

	cnt = 0;
	i = 0;
	while (rows[i])
	{
		j = 0;
		while (rows[i][j])
		{
			if (rows[i][j] == 'C')
				cnt++;
			j++;
		}
		i++;
	}
	return (cnt);
}
