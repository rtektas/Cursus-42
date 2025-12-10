/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assets_and_path.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 15:07:00 by rtektas           #+#    #+#             */
/*   Updated: 2025/09/17 15:07:00 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "game.h"

/* forward decls from other compilation units */
void		check_file_access(char *path);
void		check_xpm_player_and_door(void);
void		validate_ber_extension(char *path);

static void	check_xpm_base(void)
{
	int	fd_bg;
	int	fd_block;
	int	fd_coin;

	fd_bg = io_open_ro("./sprites/floor_black_64.xpm", NULL);
	fd_block = io_open_ro("./sprites/wall_64.xpm", NULL);
	fd_coin = io_open_ro("./sprites/coin_64.xpm", NULL);
	if (fd_bg <= 0 || fd_block <= 0 || fd_coin <= 0)
	{
		err_print(ERR_XPM_MISSING);
		if (fd_bg > 0)
			close(fd_bg);
		if (fd_block > 0)
			close(fd_block);
		if (fd_coin > 0)
			close(fd_coin);
		exit(1);
	}
	close(fd_bg);
	close(fd_block);
	close(fd_coin);
}

/* moved helpers are in assets_checks.c and path_validate.c */

void	reject_empty_line(char line)
{
	if (line == '\n' || line == '\r')
	{
		err_print(ERR_MAP_NOT_RECT);
		exit(1);
	}
}

void	check_file_access(char *path)
{
	int	fd;
	int	code;

	fd = io_open_ro(path, &code);
	if (fd == -1)
	{
		if (code == ERR_MAP_PERM)
			err_print(ERR_MAP_PERM);
		else
			err_print(ERR_MAP_NOT_FOUND);
		exit(1);
	}
	close(fd);
}

/* Public small facades */
void	validate_assets_presence(void)
{
	check_xpm_base();
	check_xpm_player_and_door();
}

void	validate_input_path(char *path)
{
	validate_ber_extension(path);
	check_file_access(path);
}
