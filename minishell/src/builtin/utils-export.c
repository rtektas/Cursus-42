/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils-export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:07:29 by asaracut          #+#    #+#             */
/*   Updated: 2025/10/24 18:26:43 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	cmp_env(const void *a, const void *b)
{
	const char	*s1;
	const char	*s2;

	s1 = *(const char **)a;
	s2 = *(const char **)b;
	return (ft_strcmp(s1, s2));
}

void	extract_key(char *arg, char *key, char *eq)
{
	int	i;

	i = 0;
	while (arg[i] && &arg[i] != eq && i < 1023)
	{
		key[i] = arg[i];
		i++;
	}
	key[i] = '\0';
}

void	print_key_until_eq(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
	{
		ft_printf("%c", str[i]);
		i++;
	}
}
