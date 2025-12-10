/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_validate.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 00:00:00 by rtektas           #+#    #+#             */
/*   Updated: 2025/11/14 00:00:00 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "game.h"

static int	has_ber_ext(char *path)
{
	int	len;

	len = (int)ft_strlen(path);
	if (len < 4)
		return (0);
	if (path[len - 1] != 'r')
		return (0);
	if (path[len - 2] != 'e')
		return (0);
	if (path[len - 3] != 'b')
		return (0);
	if (path[len - 4] != '.')
		return (0);
	return (1);
}

static int	basename_is_hidden(char *path)
{
	char	*name;
	char	*p;

	name = path;
	p = path;
	while (*p)
		p++;
	while (p > name && *p != '/' && *p != '\\')
		p--;
	if (*p == '/' || *p == '\\')
		name = p + 1;
	return (name[0] == '.');
}

void	validate_ber_extension(char *path)
{
	if (!path || basename_is_hidden(path) || !has_ber_ext(path))
	{
		err_print(ERR_MAP_BAD_EXT);
		exit(1);
	}
}
