/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_emit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 13:28:47 by marvin            #+#    #+#             */
/*   Updated: 2025/11/14 13:28:47 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ps.h"

// kept as a low-level utility if ever needed elsewhere
void	ps_puts(const char *s)
{
	while (*s)
	{
		write(1, s, 1);
		s++;
	}
}
