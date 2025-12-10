/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 00:00:00 by marvin            #+#    #+#             */
/*   Updated: 2025/11/18 00:00:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ps.h"

bool	deque_is_sorted(const t_stack *s)
{
	t_node	*cur;

	if (s->size <= 1)
		return (true);
	cur = s->head;
	while (cur && cur->next)
	{
		if (cur->idx > cur->next->idx)
			return (false);
		cur = cur->next;
	}
	return (true);
}
