/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lib2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:29:29 by asaracut          #+#    #+#             */
/*   Updated: 2025/12/07 03:35:39 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cube.h"

int	ft_atoi(const char *str)
{
	int		i;
	int		symbole;
	size_t	result;

	result = 0;
	symbole = 1;
	i = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			symbole *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9' && result < LONG_MAX)
	{
		result = result * 10 + (str[i] - 48);
		i++;
	}
	if (result > LONG_MAX && symbole == 1)
		return (-1);
	else if (result > LONG_MAX)
		return (0);
	return (result * symbole);
}

char	**ft_realloc_matrix(char **old, int old_size, int new_size)
{
	char	**new;
	int		i;

	new = malloc(sizeof(char *) * (new_size + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (i < old_size)
	{
		new[i] = old[i];
		i++;
	}
	while (i < new_size + 1)
	{
		new[i] = NULL;
		i++;
	}
	free(old);
	return (new);
}
