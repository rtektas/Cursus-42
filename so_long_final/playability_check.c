/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playability_check.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 14:20:00 by rtektas           #+#    #+#             */
/*   Updated: 2025/09/17 14:20:00 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static t_point	find_player(char **grid, int h, int w)
{
	t_point	p;
	int		i;
	int		j;

	p.y = -1;
	p.x = -1;
	i = 0;
	while (i < h)
	{
		j = 0;
		while (j < w)
		{
			if (grid[i][j] == 'P')
			{
				p.y = i;
				p.x = j;
				return (p);
			}
			j++;
		}
		i++;
	}
	return (p);
}

/* dup_map and free_map moved to map_utils.c */

static void	dfs_mark(t_dfsctx *ctx, int y, int x)
{
	char	c;

	if (y < 0 || x < 0 || y >= ctx->h || x >= ctx->w)
		return ;
	c = ctx->g[y][x];
	if (c == '1' || c == 'V')
		return ;
	if (c == 'E')
		ctx->exits_seen = 1;
	if (c == 'C')
		ctx->collected++;
	ctx->g[y][x] = 'V';
	dfs_mark(ctx, y + 1, x);
	dfs_mark(ctx, y - 1, x);
	dfs_mark(ctx, y, x + 1);
	dfs_mark(ctx, y, x - 1);
}

static int	playable_via_dfs(t_map *map)
{
	t_dfsctx	ctx;
	t_point		start;
	int			total_coins;

	ctx.g = dup_map(map->mappin, map->h, map->w);
	if (!ctx.g)
		return (0);
	start = find_player(ctx.g, map->h, map->w);
	if (start.y < 0)
	{
		free_map(ctx.g, map->h);
		return (0);
	}
	ctx.collected = 0;
	ctx.exits_seen = 0;
	ctx.h = map->h;
	ctx.w = map->w;
	total_coins = map->cb;
	dfs_mark(&ctx, start.y, start.x);
	free_map(ctx.g, map->h);
	return (ctx.exits_seen && ctx.collected == total_coins);
}

int	check_playability(t_win *jeu)
{
	return (playable_via_dfs(jeu->map));
}
