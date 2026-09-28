/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 20:58:16 by asaracut          #+#    #+#             */
/*   Updated: 2025/12/07 03:48:13 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cube.h"

int	check_extension(char *map_name)
{
	size_t	len;
	char	*ext;

	len = ft_strlen(map_name);
	if (len < 4)
		return (0);
	ext = map_name + len - 4;
	if (ft_strcmp(ext, ".cub") != 0)
		return (0);
	return (1);
}

int	main(int ac, char **av)
{
	t_cub	cub;

	if (ac != 2)
		exit_error("Invalid number of arguments", 1);
	if (!check_extension(av[1]))
		exit_error("Map file must be a *.cub", 1);
	ft_memset(&cub, 0, sizeof(t_cub));
	if (parsing(av[1], &cub))
		return (write(2, "Parsing failed\n", 15), 1);
	init_player(&cub, cub.map.player_dir);
	if (start_game(&cub) != 0)
		write(2, "Failed to start game\n", 21);
	free_all(&cub);
	return (0);
}

/*                                 pour afficher les infos de la map
void	print_map(t_cub *cub)
{
	int	i;

	printf("Map:\n");
	for (i = 0; i < cub->map.height_map; i++)
	{
		printf("%s\n", cub->map.map[i]);
	}
}
void	print_info(t_cub *cub)
{
	printf("NO Texture: %s\n", cub->map.no_texture);
	printf("SO Texture: %s\n", cub->map.so_texture);
	printf("WE Texture: %s\n", cub->map.we_texture);
	printf("EA Texture: %s\n", cub->map.ea_texture);
	printf("Floor Color: %d\n", cub->map.floor_color);
	printf("Sky Color: %d\n", cub->map.sky_color);
	printf("Map Width: %d\n", cub->map.width_map);
	printf("Map Height: %d\n", cub->map.height_map);
	printf("Player Position: (%d, %d)\n", cub->map.player_x, cub->map.player_y);
	printf("Player Direction: %c\n", cub->map.player_dir);
	//player info
	printf("\nPlayer Info:\n");
	printf("Position: (%f, %f)\n", cub->player.x, cub->player.y);
	printf("Direction: (%f, %f)\n", cub->player.dir_x, cub->player.dir_y);
	printf("Plane: (%f, %f)\n", cub->player.plane_x, cub->player.plane_y);
}
*/