/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_ops.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 13:28:44 by marvin            #+#    #+#             */
/*   Updated: 2025/11/14 13:28:44 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ps.h"

static void	push_from_to(t_stack *from, t_stack *to)
{
	t_node	*n;
	t_node	*nx;

	if (from->size == 0)
		return ;
	n = from->head;
	nx = n->next;
	from->head = nx;
	if (nx)
		nx->prev = NULL;
	else
		from->tail = NULL;
	from->size--;
	n->prev = NULL;
	n->next = to->head;
	if (to->head)
		to->head->prev = n;
	to->head = n;
	if (!to->tail)
		to->tail = n;
	to->size++;
}

void	op_pa(t_stack *a, t_stack *b)
{
	push_from_to(b, a);
	ps_emit_move("pa\n");
}

void	op_pb(t_stack *b, t_stack *a)
{
	push_from_to(a, b);
	ps_emit_move("pb\n");
}
