/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 02:26:53 by asaracut          #+#    #+#             */
/*   Updated: 2025/12/07 03:37:45 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cube.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char	*add_char_line(char *line, char c)
{
	size_t	i;
	char	*new_line;

	if (!line)
		i = 0;
	else
		i = ft_strlen(line);
	new_line = (char *)malloc(i + 2);
	if (!new_line)
	{
		free(line);
		return (NULL);
	}
	if (line)
	{
		ft_memcpy(new_line, line, i);
		free(line);
	}
	new_line[i] = c;
	new_line[i + 1] = '\0';
	return (new_line);
}

char	*get_next_line(int fd)
{
	char	buf;
	ssize_t	read_ret;
	char	*line;

	if (fd < 0)
		return (NULL);
	line = NULL;
	read_ret = read(fd, &buf, 1);
	while (read_ret > 0)
	{
		line = add_char_line(line, buf);
		if (!line)
			return (NULL);
		if (buf == '\n')
			break ;
		read_ret = read(fd, &buf, 1);
	}
	if (read_ret <= 0 && (!line || line[0] == '\0'))
	{
		free(line);
		return (NULL);
	}
	return (line);
}
