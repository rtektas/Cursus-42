/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bit_radix_sort.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 13:29:12 by marvin            #+#    #+#             */
/*   Updated: 2025/11/14 13:29:12 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ps.h"

static int	max_bits_needed(const t_stack *a)
{
	int		max;
	int		bits;
	t_node	*n;

	max = 0;
	n = a->head;
	while (n)
	{
		if (n->idx > max)
			max = n->idx;
		n = n->next;
	}
	bits = 0;
	while ((max >> bits) != 0)
		bits++;
	return (bits);
}

void	sort_radix(t_stack *a, t_stack *b)
{
	int	bits;
	int	count;
	int	top;
	int	bit;

	bits = max_bits_needed(a);
	bit = 0;
	while (bit < bits)
	{
		count = a->size;
		while (count > 0)
		{
			top = a->head->idx;
			if ((top >> bit) & 1)
				op_ra(a);
			else
				op_pb(b, a);
			count--;
		}
		while (b->size > 0)
			op_pa(a, b);
		bit++;
	}
}
