/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index_assign.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 13:28:52 by marvin            #+#    #+#             */
/*   Updated: 2025/11/14 13:28:52 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ps.h"

// O(n^2) rank normalization: idx = number of strictly smaller values
void	rank_normalize(t_stack *a)
{
	t_node	*x;
	t_node	*y;
	int		cnt;

	x = a->head;
	while (x)
	{
		cnt = 0;
		y = a->head;
		while (y)
		{
			if (y->val < x->val)
				cnt++;
			y = y->next;
		}
		x->idx = cnt;
		x = x->next;
	}
}
