/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 00:00:00 by rtektas           #+#    #+#             */
/*   Updated: 2025/11/14 00:00:00 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "game.h"

void		reject_empty_line(char line);

static void	map_exit_bad(t_map *ret, char *raw)
{
	if (raw)
		free(raw);
	if (ret)
		free(ret);
	err_print(ERR_MAP_NOT_RECT);
	exit(1);
}

static char	**split_lines(const char *s)
{
	char	**arr;

	if (!s || !*s)
		return (NULL);
	arr = ft_split(s, '\n');
	return (arr);
}

static void	handle_line(char **pret, char *line)
{
	if (line[0] == '\r' && (line[1] == '\n' || line[1] == '\0'))
		reject_empty_line('\n');
	else
		reject_empty_line(line[0]);
	*pret = join_and_free(*pret, line);
}

static char	*read_file(char *path)
{
	int		fd;
	int		code;
	char	*ret;
	char	*line;

	fd = io_open_ro(path, &code);
	if (fd < 0)
	{
		err_print(code);
		exit(1);
	}
	ret = NULL;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		handle_line(&ret, line);
		free(line);
	}
	close(fd);
	return (ret);
}

t_map	*read_map(char *path)
{
	char	*raw;
	t_map	*ret;

	ret = ft_calloc(1, sizeof(t_map));
	raw = read_file(path);
	if (!raw || !*raw)
		map_exit_bad(ret, raw);
	ret->mappin = split_lines(raw);
	if (!ret->mappin || !ret->mappin[0])
		map_exit_bad(ret, raw);
	ret->h = 0;
	while (ret->mappin[ret->h])
		ret->h++;
	ret->w = (int)ft_strlen(ret->mappin[0]);
	ret->cb = count_coins(ret->mappin);
	free(raw);
	return (ret);
}
