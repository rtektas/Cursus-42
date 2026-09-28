/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 21:52:47 by asaracut          #+#             */
/*   Updated: 2025/11/25 02:39:31 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cube.h"

int	verifi_line(char *line, t_cub *cub)
{
	if (cub->map.parsing_step < 6 && empty_line(line))
		return (0);
	else if (cub->map.parsing_step == 6)
	{
		if (!empty_line(line))
			cub->map.parsing_step++;
		else
			return (0);
	}
	if (cub->map.parsing_step < 6)
	{
		stock_info(line, cub);
		cub->map.parsing_step++;
	}
	else
		stock_map(line, cub);
	return (0);
}

int	read_map(int fd, t_cub *cub)
{
	char	*line;

	line = get_next_line(fd);
	while (line != NULL)
	{
		if (verifi_line(line, cub) != 0)
		{
			free(line);
			return (-1);
		}
		free(line);
		line = get_next_line(fd);
	}
	return (0);
}

int	parsing(char *map, t_cub *cub)
{
	int	fd;

	fd = open(map, O_RDONLY);
	if (fd < 0)
		exit_error("Could not open map file", 1);
	if (read_map(fd, cub) != 0)
		exit_error("Could not read map file", 1);
	close(fd);
	parse_map(cub);
	write(1, "\n✅ Parsing successful ✅\n", 29);
	return (0);
}

/*
int	ligne = 1;
int	verifi_line(char *line, t_cub *cub)
{
	if (cub->map.parsing_step < 6 && empty_line(line))
	{
		printf("ligne %d vide\n", ligne++);
		return (0);
	}
	else if (cub->map.parsing_step == 6)
	{
		if (!empty_line(line))
			cub->map.parsing_step++;
		else
		{
			printf("ligne %d vide\n", ligne++);
			return (0);
		}
	}
	if (cub->map.parsing_step < 6)
	{
		printf("ligne %d parsing map etape %d: %s", ligne++,
			cub->map.parsing_step, line);
		stock_info(line, cub);
		cub->map.parsing_step++;
	}
	else
	{
		printf("ligne %d parsing map etape %d: %s", ligne++,
			cub->map.parsing_step, line);
		stock_map(line, cub);
	}
	return (0);
}

int	read_map(int fd, t_cub *cub)
{
	char	*line;

	while ((line = get_next_line(fd)) != NULL)
	{
		if (verifi_line(line, cub) != 0)
		{
			free(line);
			return (-1);
		}
		free(line);
	}
	return (0);
}

int	parsing(char *map, t_cub *cub)
{
	int	fd;

	fd = open(map, O_RDONLY);
	if (fd < 0)
		exit_error("Could not open map file", 1); // a rajouter free si besoin
	if (read_map(fd, cub) != 0)
		exit_error("Could not read map file", 1);
	close(fd);
	parse_map(cub);
	write(1, "Parsing successful\n", 20);
	return (0);
}
*/