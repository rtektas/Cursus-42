/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stock_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 02:35:54 by asaracut          #+#    #+#             */
/*   Updated: 2025/12/07 04:21:40 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cube.h"

static void	parse_path(char *line, t_cub *cub,
		size_t *i, char **texture_path)
{
	size_t	start;
	size_t	end;

	*i = 0;
	while (line[*i] == ' ' || line[*i] == '\t')
		(*i)++;
	start = *i;
	while (line[*i] && line[*i] != '\n'
		&& line[*i] != ' ' && line[*i] != '\t')
		(*i)++;
	end = *i;
	if (end == start)
	{
		free_textures(cub);
		exit_error("Empty texture path", 1);
	}
	*texture_path = (char *)malloc(end - start + 1);
	if (!*texture_path)
	{
		free_textures(cub);
		exit_error("Memory allocation failed for texture path", 1);
	}
	ft_memcpy(*texture_path, line + start, end - start);
	(*texture_path)[end - start] = '\0';
}

static void	check_extension(char *texture_path, t_cub *cub)
{
	size_t	len;

	len = ft_strlen(texture_path);
	if (len < 5 || ft_strcmp(texture_path + len - 4, ".xpm") != 0)
	{
		free(texture_path);
		free_textures(cub);
		exit_error("Texture must end with .xpm", 1);
	}
}

static void	check_trailing_chars(char *line, size_t i,
		char *texture_path, t_cub *cub)
{
	while (line[i] && line[i] != '\n')
	{
		if (line[i] != ' ' && line[i] != '\t')
		{
			free(texture_path);
			free_textures(cub);
			exit_error("Invalid characters after .xpm", 1);
		}
		i++;
	}
}

static void	open_and_store(char *texture_path, t_cub *cub)
{
	int	fd;

	fd = open(texture_path, O_RDONLY);
	if (fd < 0)
	{
		free(texture_path);
		free_textures(cub);
		exit_error("Cannot open texture file", 1);
	}
	close(fd);
	if (cub->map.parsing_step == 0)
		cub->map.no_texture = texture_path;
	else if (cub->map.parsing_step == 1)
		cub->map.so_texture = texture_path;
	else if (cub->map.parsing_step == 2)
		cub->map.we_texture = texture_path;
	else if (cub->map.parsing_step == 3)
		cub->map.ea_texture = texture_path;
	else
	{
		free(texture_path);
		free_textures(cub);
		exit_error("Invalid parsing step for texture", 1);
	}
}

int	stock_texture(char *line, t_cub *cub)
{
	size_t	i;
	char	*texture_path;

	parse_path(line, cub, &i, &texture_path);
	check_extension(texture_path, cub);
	check_trailing_chars(line, i, texture_path, cub);
	open_and_store(texture_path, cub);
	return (0);
}
