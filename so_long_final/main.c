/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 12:00:00 by rtektas           #+#    #+#             */
/*   Updated: 2025/11/14 16:49:49 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "game.h"

/* Prototypes fournis par jeu.h pour le démarrage */
void	start_full_game(t_win *pnc, char *path);

/* Point d'entrée: on vérifie les arguments,
	on prépare et on lance la boucle. */
int	main(int argc, char **argv)
{
	t_win	pnc;

	if (argc == 2)
	{
		ft_bzero(&pnc, sizeof(t_win));
		validate_assets_presence();
		validate_input_path(argv[1]);
		start_full_game(&pnc, argv[1]);
		render_map(&pnc);
		pnc.input = (t_input){0};
		pnc.has_action = 0;
		pnc.last_key = 0;
		pnc.door_open = (pnc.map->cb <= 0);
		pnc.pending_exit = 0;
		mlx_hook(pnc.win, 2, (1L << 0), on_key_down, &pnc);
		mlx_hook(pnc.win, 3, (1L << 1), on_key_up, &pnc);
		mlx_loop_hook(pnc.mlx, game_tick, &pnc);
		mlx_hook(pnc.win, 17, 0, close_frame, &pnc);
		mlx_loop(pnc.mlx);
	}
	else
	{
		err_print(ERR_ARGS_COUNT);
	}
	return (0);
}
