/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stock_info.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 02:17:47 by asaracut          #+#    #+#             */
/*   Updated: 2025/12/07 03:55:43 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cube.h"

void	stock_info_loop(char **line, t_cub *cub, char *direction)
{
	skip_spaces(line);
	if (ft_strncmp(*line, direction, 3) != 0)
	{
		free_textures(cub);
		exit_error("Wrong texture format", 1);
	}
	*line += 3;
	skip_spaces(line);
	stock_texture(*line, cub);
}

void	stock_info_loop_color(char **line, t_cub *cub, char *color_type)
{
	skip_spaces(line);
	if (ft_strncmp(*line, color_type, 2) != 0)
	{
		free_textures(cub);
		exit_error("Wrong floor color format", 1);
	}
	*line += 2;
	stock_color(*line, cub);
}

int	stock_info(char *line, t_cub *cub)
{
	if (cub->map.parsing_step == 0)
		stock_info_loop(&line, cub, "NO ");
	else if (cub->map.parsing_step == 1)
		stock_info_loop(&line, cub, "SO ");
	else if (cub->map.parsing_step == 2)
		stock_info_loop(&line, cub, "WE ");
	else if (cub->map.parsing_step == 3)
		stock_info_loop(&line, cub, "EA ");
	else if (cub->map.parsing_step == 4)
		stock_info_loop_color(&line, cub, "F ");
	else if (cub->map.parsing_step == 5)
		stock_info_loop_color(&line, cub, "C ");
	return (0);
}
