/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_stack.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 13:29:02 by marvin            #+#    #+#             */
/*   Updated: 2025/11/14 13:29:02 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ps.h"

static t_node	*node_new(int val)
{
	t_node	*n;

	n = (t_node *)malloc(sizeof(t_node));
	if (!n)
		return (NULL);
	n->val = val;
	n->idx = -1;
	n->prev = NULL;
	n->next = NULL;
	return (n);
}

void	deque_init(t_stack *s, char name)
{
	s->head = NULL;
	s->tail = NULL;
	s->size = 0;
	s->name = name;
}

void	deque_clear(t_stack *s)
{
	t_node	*cur;
	t_node	*nx;

	cur = s->head;
	while (cur)
	{
		nx = cur->next;
		free(cur);
		cur = nx;
	}
	s->head = NULL;
	s->tail = NULL;
	s->size = 0;
}

void	deque_push_head(t_stack *s, int val)
{
	t_node	*n;

	n = node_new(val);
	if (!n)
		return ;
	n->next = s->head;
	if (s->head)
		s->head->prev = n;
	s->head = n;
	if (!s->tail)
		s->tail = n;
	s->size++;
}

void	deque_push_tail(t_stack *s, int val)
{
	t_node	*n;

	n = node_new(val);
	if (!n)
		return ;
	n->prev = s->tail;
	if (s->tail)
		s->tail->next = n;
	s->tail = n;
	if (!s->head)
		s->head = n;
	s->size++;
}

/* deque_is_sorted moved to deque_utils.c to satisfy Norm function count */
