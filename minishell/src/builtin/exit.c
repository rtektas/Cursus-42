/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:05:37 by asaracut          #+#    #+#             */
/*   Updated: 2025/10/24 18:26:43 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_numeric_arg_valid(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(char **args)
{
	int	status;

	ft_printf("exit\n");
	if (!args[1])
		exit(0);
	if (!is_numeric_arg_valid(args[1]))
	{
		ft_printf("exit: %s: numeric argument required\n", args[1]);
		exit(255);
	}
	if (args[2])
	{
		ft_printf("exit: too many arguments\n");
		return (1);
	}
	status = ft_atoi(args[1]) % 256;
	exit((unsigned char)status);
	return (status);
}
