/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rrotate_ops.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 13:28:36 by marvin            #+#    #+#             */
/*   Updated: 2025/11/14 13:28:36 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ps.h"

static void	rev_rotate_stack(t_stack *s)
{
	t_node	*old_tail;
	t_node	*new_tail;

	if (s->size < 2)
		return ;
	old_tail = s->tail;
	new_tail = old_tail->prev;
	new_tail->next = NULL;
	s->tail = new_tail;
	old_tail->prev = NULL;
	old_tail->next = s->head;
	if (s->head)
		s->head->prev = old_tail;
	s->head = old_tail;
}

void	op_rra(t_stack *a)
{
	rev_rotate_stack(a);
	ps_emit_move("rra\n");
}

void	op_rrb(t_stack *b)
{
	rev_rotate_stack(b);
	ps_emit_move("rrb\n");
}

void	op_rrr(t_stack *a, t_stack *b)
{
	rev_rotate_stack(a);
	rev_rotate_stack(b);
	ps_emit_move("rrr\n");
}
