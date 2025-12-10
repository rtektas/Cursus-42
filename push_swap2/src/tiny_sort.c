/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tiny_sort.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 13:28:26 by marvin            #+#    #+#             */
/*   Updated: 2025/11/14 13:28:26 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ps.h"

static void	dual_sort(t_stack *a)
{
	if (a->size < 2)
		return ;
	if (a->head->idx > a->head->next->idx)
		op_sa(a);
}

static void	triad_sort(t_stack *a)
{
	int	x;
	int	y;
	int	z;

	x = a->head->idx;
	y = a->head->next->idx;
	z = a->tail->idx;
	if (x < y && y < z)
		return ;
	if (x > y && y < z && x < z)
		op_sa(a);
	else if (x > y && y > z)
	{
		op_sa(a);
		op_rra(a);
	}
	else if (x > z && y < z)
		op_ra(a);
	else if (x < y && x > z)
		op_rra(a);
	else
	{
		op_sa(a);
		op_ra(a);
	}
}

static int	find_min_rank_pos(t_stack *a)
{
	int		pos;
	int		best;
	int		best_pos;
	t_node	*n;

	pos = 0;
	best_pos = 0;
	best = INT_MAX;
	n = a->head;
	while (n)
	{
		if (n->idx < best)
		{
			best = n->idx;
			best_pos = pos;
		}
		pos++;
		n = n->next;
	}
	return (best_pos);
}

static void	rise_min_to_top(t_stack *a)
{
	int	pos;
	int	half;
	int	k;

	pos = find_min_rank_pos(a);
	half = a->size / 2;
	if (pos <= half)
	{
		while (pos--)
			op_ra(a);
	}
	else
	{
		k = a->size - pos;
		while (k--)
			op_rra(a);
	}
}

void	small_block_sort(t_stack *a, t_stack *b)
{
	if (a->size == 2)
	{
		dual_sort(a);
		return ;
	}
	if (a->size == 3)
	{
		triad_sort(a);
		return ;
	}
	while (a->size > 3)
	{
		rise_min_to_top(a);
		op_pb(b, a);
	}
	triad_sort(a);
	while (b->size > 0)
		op_pa(a, b);
}
