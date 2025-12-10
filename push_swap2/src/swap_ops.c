/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap_ops.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 13:28:31 by marvin            #+#    #+#             */
/*   Updated: 2025/11/14 13:28:31 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ps.h"

static void	swap_stack(t_stack *s)
{
	t_node	*a;
	t_node	*b;
	t_node	*c;

	if (s->size < 2)
		return ;
	a = s->head;
	b = a->next;
	c = b->next;
	b->prev = NULL;
	b->next = a;
	a->prev = b;
	a->next = c;
	if (c)
		c->prev = a;
	s->head = b;
	if (s->tail == b)
		s->tail = a;
}

void	op_sa(t_stack *a)
{
	swap_stack(a);
	ps_emit_move("sa\n");
}

void	op_sb(t_stack *b)
{
	swap_stack(b);
	ps_emit_move("sb\n");
}

void	op_ss(t_stack *a, t_stack *b)
{
	swap_stack(a);
	swap_stack(b);
	ps_emit_move("ss\n");
}
