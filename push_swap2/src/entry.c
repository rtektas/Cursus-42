/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entry.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 13:28:58 by marvin            #+#    #+#             */
/*   Updated: 2025/11/14 13:28:58 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ps.h"

static void	dispatch_sort(t_stack *a, t_stack *b)
{
	if (a->size <= 1)
		return ;
	if (deque_is_sorted(a))
		return ;
	if (a->size <= 5)
		small_block_sort(a, b);
	else
		sort_radix(a, b);
}

int	main(int argc, char **argv)
{
	t_stack	a;
	t_stack	b;

	deque_init(&a, 'a');
	deque_init(&b, 'b');
	if (argc < 2)
		return (0);
	if (ps_parse_args_build(&a, argc, argv) != 0)
	{
		deque_clear(&a);
		deque_clear(&b);
		return (ps_error());
	}
	rank_normalize(&a);
	dispatch_sort(&a, &b);
	ps_flush_moves();
	deque_clear(&a);
	deque_clear(&b);
	return (0);
}
