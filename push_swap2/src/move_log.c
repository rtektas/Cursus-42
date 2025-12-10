/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_log.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 13:28:40 by marvin            #+#    #+#             */
/*   Updated: 2025/11/18 00:00:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ps.h"

/*
** Version sans variables globales: on écrit directement chaque mouvement.
** ps_flush_moves() devient un no-op pour préserver l'API.
*/

void	ps_emit_move(const char *mnemonic)
{
	size_t	len;

	len = 0;
	while (mnemonic[len])
		len++;
	if (len > 0)
		write(1, mnemonic, len);
}

void	ps_flush_moves(void)
{
}
