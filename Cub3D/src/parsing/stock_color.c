/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stock_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 23:17:56 by asaracut          #+#    #+#             */
/*   Updated: 2025/12/07 04:18:03 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cube.h"

static int	parse_component(char **line, t_cub *cub)
{
	int	value;

	skip_spaces(line);
	if (**line < '0' || **line > '9')
		free_texture_exit(cub, "Invalid color format");
	value = ft_atoi(*line);
	while (**line >= '0' && **line <= '9')
		(*line)++;
	return (value);
}

static void	verifi_separator(char **line, t_cub *cub)
{
	skip_spaces(line);
	if (**line != ',')
		free_texture_exit(cub, "Invalid color format");
	(*line)++;
}

static void	verifi_color_range(int r, int g, int b, t_cub *cub)
{
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		free_texture_exit(cub, "Color values must be between 0 and 255");
}

int	stock_color(char *line, t_cub *cub)
{
	int	r;
	int	g;
	int	b;

	r = parse_component(&line, cub);
	verifi_separator(&line, cub);
	g = parse_component(&line, cub);
	verifi_separator(&line, cub);
	b = parse_component(&line, cub);
	skip_spaces(&line);
	if (*line != '\0' && *line != '\n')
		free_texture_exit(cub, "Invalid color format");
	verifi_color_range(r, g, b, cub);
	if (cub->map.parsing_step == 4)
		cub->map.floor_color = (r << 16) | (g << 8) | b;
	else if (cub->map.parsing_step == 5)
		cub->map.sky_color = (r << 16) | (g << 8) | b;
	return (0);
}
