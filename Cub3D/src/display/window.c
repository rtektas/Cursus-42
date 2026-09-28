/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 03:35:38 by asaracut          #+#    #+#             */
/*   Updated: 2025/12/07 03:59:53 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cube.h"

int	handle_exit(t_cub *cub)
{
	free_all(cub);
	if (cub->tex_no.img)
		mlx_destroy_image(cub->img.mlx_ptr, cub->tex_no.img);
	if (cub->tex_so.img)
		mlx_destroy_image(cub->img.mlx_ptr, cub->tex_so.img);
	if (cub->tex_we.img)
		mlx_destroy_image(cub->img.mlx_ptr, cub->tex_we.img);
	if (cub->tex_ea.img)
		mlx_destroy_image(cub->img.mlx_ptr, cub->tex_ea.img);
	if (cub->img.img)
		mlx_destroy_image(cub->img.mlx_ptr, cub->img.img);
	if (cub->img.win_ptr)
		mlx_destroy_window(cub->img.mlx_ptr, cub->img.win_ptr);
	printf("Exiting...\n");
	exit(0);
	return (0);
}

int	handle_key(int key, t_cub *cub)
{
	printf("Key pressed: %d\n", key);
	cub->move.nx = cub->player.x;
	cub->move.ny = cub->player.y;
	if (key == 97)
		move_player_a(cub);
	else if (key == 100)
		move_player_d(cub);
	else if (key == 65362 || key == 119)
		move_player_w(cub);
	else if (key == 65364 || key == 115)
		move_player_s(cub);
	else if (key == 65361)
		rotation(-1, cub);
	else if (key == 65363)
		rotation(1, cub);
	move_valid_or_no(cub);
	if (key == 65307)
		handle_exit(cub);
	return (0);
}

int	start_game(t_cub *cub)
{
	cub->img.mlx_ptr = mlx_init();
	if (!cub->img.mlx_ptr)
		return (1);
	cub->img.win_ptr = mlx_new_window(cub->img.mlx_ptr, 800, 600, "Test MLX");
	if (!cub->img.win_ptr)
		return (1);
	load_textures(cub);
	render_frame(cub, cub->img.mlx_ptr, cub->img.win_ptr);
	mlx_hook(cub->img.win_ptr, 2, 1L << 0, handle_key, cub);
	mlx_hook(cub->img.win_ptr, 17, 1L << 17, handle_exit, cub);
	mlx_loop(cub->img.mlx_ptr);
	return (0);
}

/*
void	move_pixel(t_cub *cub, void *mlx, void *win)
		// juste pour tester le deplacement, a effacer apres
{
	cub->img.img = mlx_new_image(mlx, 800, 600);
	cub->img.addr = mlx_get_data_addr(cub->img.img, &cub->img.bpp,
			&cub->img.line_len, &cub->img.endian);
	mlx_put_image_to_window(mlx, win, cub->img.img, 0, 0);

	put_pixel(&cub->img, test[0], test[1], 0xFFFFFF);

	mlx_put_image_to_window(mlx, win, cub->img.img, 0, 0);
}*/