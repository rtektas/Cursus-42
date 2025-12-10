/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate_ops.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 13:28:41 by marvin            #+#    #+#             */
/*   Updated: 2025/11/14 13:28:41 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ps.h"

static void	rotate_stack(t_stack *s)
{
	t_node	*old_head;
	t_node	*new_head;

	if (s->size < 2)
		return ;
	old_head = s->head;
	new_head = old_head->next;
	new_head->prev = NULL;
	s->head = new_head;
	old_head->next = NULL;
	old_head->prev = s->tail;
	if (s->tail)
		s->tail->next = old_head;
	s->tail = old_head;
}

void	op_ra(t_stack *a)
{
	rotate_stack(a);
	ps_emit_move("ra\n");
}

void	op_rb(t_stack *b)
{
	rotate_stack(b);
	ps_emit_move("rb\n");
}

void	op_rr(t_stack *a, t_stack *b)
{
	rotate_stack(a);
	rotate_stack(b);
	ps_emit_move("rr\n");
}
