/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 16:36:57 by marvin            #+#    #+#             */
/*   Updated: 2025/11/14 16:36:57 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include <errno.h>

static const char	*get_err_msg(int code)
{
	static const char	*msgs[] = {"", "Error\nNo exist map\n",
		"Error\nPermission denied\n", "Error\nBad extension\n",
		"Error\nNo rectangular\n", "Error\nNot surrounded by walls\n",
		"Error\nWrong characters\n", "Error\nNo player\n",
		"Error\nDuplicate player\n", "Error\nNo exit\n",
		"Error\nDuplicate exit\n", "Error\nNo object\n",
		"Error\nMissing XPM File\n", "Error\nDuplicate exit\n",
		"Error\nInvalid XPM content or size\n",
		"Error\nWrong number of arguments\n"};
	int					count;

	count = (int)(sizeof(msgs) / sizeof(msgs[0]));
	if (code < 0 || code >= count)
		return ("");
	return (msgs[code]);
}

void	err_print(int code)
{
	const char	*msg;
	int			len;

	msg = get_err_msg(code);
	if (msg && *msg)
	{
		len = 0;
		while (msg[len])
			len++;
		write(1, msg, len);
	}
}

int	io_open_ro(const char *path, int *out_code)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		if (out_code)
		{
			if (errno == EACCES)
				*out_code = ERR_MAP_PERM;
			else
				*out_code = ERR_MAP_NOT_FOUND;
		}
		return (-1);
	}
	if (out_code)
		*out_code = ERR_OK;
	return (fd);
}

int	io_open_rw(const char *path, int *out_code)
{
	int	fd;

	fd = open(path, O_RDWR);
	if (fd < 0)
	{
		if (out_code)
			*out_code = ERR_MAP_PERM;
		return (-1);
	}
	if (out_code)
		*out_code = ERR_OK;
	return (fd);
}

ssize_t	io_safe_write(int fd, const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (write(fd, s, (int)len));
}
