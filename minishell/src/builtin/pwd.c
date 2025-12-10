/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:04:05 by asaracut          #+#    #+#             */
/*   Updated: 2025/10/24 18:26:43 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_pwd(char **args)
{
	char	cwd[100];

	(void)args;
	if (getcwd(cwd, sizeof(cwd)))
	{
		ft_printf("%s\n", cwd);
		return (0);
	}
	perror("pwd");
	return (1);
}
