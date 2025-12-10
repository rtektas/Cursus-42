/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 09:00:00 by rtektas           #+#    #+#             */
/*   Updated: 2025/09/23 09:00:00 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# include <fcntl.h>
# include <unistd.h>

/* Codes d'erreur maison */
# define ERR_OK 0
# define ERR_MAP_NOT_FOUND 1
# define ERR_MAP_PERM 2
# define ERR_MAP_BAD_EXT 3
# define ERR_MAP_NOT_RECT 4
# define ERR_MAP_OPEN_BOUND 5
# define ERR_MAP_BAD_CHARS 6
# define ERR_MAP_NO_PLAYER 7
# define ERR_MAP_MULTI_PLAYER 8
# define ERR_MAP_NO_EXIT 9
# define ERR_MAP_MULTI_EXIT 10
# define ERR_MAP_NO_COIN 11
# define ERR_XPM_MISSING 12
# define ERR_MAP_UNSOLVABLE 13
# define ERR_XPM_INVALID 14
# define ERR_ARGS_COUNT 15

/* Affichage message selon code */
void	err_print(int code);

/* Wrappers I/O */
int		io_open_ro(const char *path, int *out_code);
int		io_open_rw(const char *path, int *out_code);
ssize_t	io_safe_write(int fd, const char *s);

#endif
