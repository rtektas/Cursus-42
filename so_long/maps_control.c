/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maps_control.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 17:40:17 by rtektas           #+#    #+#             */
/*   Updated: 2024/10/03 17:40:20 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minlbx.h"

int	str_len(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	player_control(t_win *pnc)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (pnc->map->mappin[i])
	{
		while (pnc->map->mappin[i][j])
		{
			if (pnc->map->mappin[i][j] == 'P')
			{
				pnc->map->p_cnt += 1;
			}
			j++;
		}
		i++;
		j = 0;
	}
	if (pnc->map->p_cnt != 1)
	{
		write(1, "Player Count Error", 19);
		close_frame(pnc);
	}
}

void	wall_control(t_win *pnc)
{
	int	i;
	int	last;

	i = -1;
	while (pnc->map->mappin[0][++i])
	{
		if (pnc->map->mappin[0][i] != '1' || pnc->map->mappin[pnc->map->h
			- 1][i] != '1')
		{
			write(1, "Wall Error", 11);
			close_frame(pnc);
		}
	}
	i = -1;
	last = pnc->map->w - 1;
	while (pnc->map->mappin[++i])
	{
		if (pnc->map->mappin[i][0] != '1' || pnc->map->mappin[i][last] != '1')
		{
			write(1, "Wall Error", 11);
			close_frame(pnc);
		}
	}
}

void	map_size_control(t_win *pnc)
{
	int	i;
	int	len;

	i = 0;
	len = pnc->map->w;
	while (pnc->map->mappin[i])
	{
		if (str_len(pnc->map->mappin[i]) != len)
		{
			write(1, "Line Size Error", 16);
			close_frame(pnc);
		}
		i++;
	}
}

void	end_control(t_win *pnc)
{
	int	i;
	int	j;

	i = -1;
	j = -1;
	while (pnc->map->mappin[++i])
	{
		while (pnc->map->mappin[i][++j])
		{
			if (pnc->map->mappin[i][j] == 'E')
			{
				pnc->map->end_cnt = pnc->map->end_cnt + 1;
			}
		}
		j = 0;
	}
	if (pnc->map->end_cnt < 1)
	{
		write(1, "End Door Count Error", 21);
		close_frame(pnc);
	}
}

// static int backtracking(char **maps, int x, int y, t_chkc *epc)
// {
//     int i;
//     i = 0;

//     // Vérification des obstacles ou des cases déjà visitées
//     if (maps[y][x] == '1' || maps[y][x] == 'Z')
//         return (0);

//     // Si la case est vide, on la marque comme visitée
//     if (maps[y][x] == '0')
//         maps[y][x] = 'Z';

//     // Si on atteint la sortie, on décrémente le compteur des sorties
//     if (maps[y][x] == 'E')
//     {
//         epc->e--;
//         maps[y][x] = 'Z';
//     }

//     // Si on trouve un collectible,
//     if (maps[y][x] == 'C')
//     {
//         epc->c--;
//         maps[y][x] = 'Z';
//     }

//     // Exploration des quatre directions (haut, bas, gauche, droite)
//     backtracking(maps, x + 1, y, epc);
//     backtracking(maps, x - 1, y, epc);
//     backtracking(maps, x, y + 1, epc);
//     backtracking(maps, x, y - 1, epc);

//     // Vérification si tous les collectibles et la sortie sont atteints
//     if (epc->e == 0 && epc->c == 0)
//         return (1);

//     return (0);
// }

// static int find_axe(char **maps, t_chkc *epc)
// {
//     int x;
//     int y;

//     y = 0;
//     while (maps[y])
//     {
//         x = 0;
//         while (maps[y][x])
//         {
//             if (maps[y][x] == 'P')
//                 break ;
//             x++;
//         }

//         if (maps[y][x] == 'P')
//             break ;
//         y++;
//     }

//     return (backtracking(maps, x, y, epc));
// }

void	mpd(t_win *game)
{
	int i, j;
	game->map->mappin_copy = (char **)malloc(sizeof(char *) * game->map->h);
	for (i = 0; i < game->map->h; i++)
	{
		game->map->mappin_copy[i] = (char *)malloc(sizeof(char) * (game->map->w
					+ 1));
		for (j = 0; j < game->map->w; j++)
		{
			game->map->mappin_copy[i][j] = game->map->mappin[i][j];
			if (game->map->mappin[i][j] == 'P')
			{
				game->chr->l_y = i;
				game->chr->l_x = j;
			}
		}
		game->map->mappin_copy[i][j] = 0;
	}
}

int	the_great_flood(t_win *game, int y, int x)
{
	// Vérifier si la position est en dehors de la carte
	if (x < 0 || y < 0 || y >= game->map->h || x >= game->map->w)
		return (0);
	// Vérifier si la case est accessible
	if (game->map->mappin_copy[y][x] == '0'
		|| game->map->mappin_copy[y][x] == 'C'
		|| game->map->mappin_copy[y][x] == 'E'
		|| game->map->mappin_copy[y][x] == 'P')
	{
		if (game->map->mappin_copy[y][x] == 'E')
			game->map->end_cnt++;
		if (game->map->mappin_copy[y][x] == 'C')
			game->map->coin_cnt++;
		// Marquer la case comme visitée
		game->map->mappin_copy[y][x] = 'V';
		// Continuer à explorer dans les quatre directions
		the_great_flood(game, y + 1, x);
		the_great_flood(game, y - 1, x);
		the_great_flood(game, y, x + 1);
		the_great_flood(game, y, x - 1);
	}
	return (1);
}

#include <stdio.h>

int	go_nuts(t_win *game)
{
	int	i;

	game->map->end_cnt = 0;
	game->map->coin_cnt = 0;
	// Copier la carte pour effectuer le flood fill
	mpd(game);
	// Lancer le flood fill à partir de la position du joueur
	the_great_flood(game, game->chr->l_y, game->chr->l_x);
	game->chr->l_y = 0;
	game->chr->l_x = 0;
	// Libérer la copie de la carte
	for (i = 0; i < game->map->h; i++)
	{
		free(game->map->mappin_copy[i]);
	}
	free(game->map->mappin_copy);
	// Vérifier s'il y a exactement une sortie atteignable et que tous les collectibles sont atteints
	if (game->map->end_cnt == 1 && game->map->coin_cnt == game->map->cb)
	{
		return (1); // Carte valide
	}
	// Sinon, la carte est invalide
	return (0);
}
