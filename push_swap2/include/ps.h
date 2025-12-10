/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ps.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 20:12:17 by marvin            #+#    #+#             */
/*   Updated: 2025/11/18 20:12:17 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PS_H
# define PS_H

# include <ctype.h>
# include <limits.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>

// Minimal libft prototypes used (avoid depending on external header)
int					ft_isdigit(int c);
char				**ft_split(char const *s, char c);

// Doubly-linked node for O(1) rotate/rev-rotate
typedef struct s_node
{
	int				val;
	int				idx;
	struct s_node	*prev;
	struct s_node	*next;
}					t_node;

typedef struct s_stack
{
	t_node			*head;
	t_node			*tail;
	int				size;
	char			name;
}					t_stack;

// core/parse.c
int					ps_parse_args_build(t_stack *a, int argc, char **argv);

// ds/deque_stack.c
void				deque_init(t_stack *s, char name);
void				deque_clear(t_stack *s);
bool				deque_is_sorted(const t_stack *s);
void				deque_push_head(t_stack *s, int val);
void				deque_push_tail(t_stack *s, int val);

// utils/move_log.c
void				ps_emit_move(const char *mnemonic);
void				ps_flush_moves(void);

// ds/op_*.c (each op prints its name)
void				op_sa(t_stack *a);
void				op_sb(t_stack *b);
void				op_ss(t_stack *a, t_stack *b);
void				op_pa(t_stack *a, t_stack *b);
void				op_pb(t_stack *b, t_stack *a);
void				op_ra(t_stack *a);
void				op_rb(t_stack *b);
void				op_rr(t_stack *a, t_stack *b);
void				op_rra(t_stack *a);
void				op_rrb(t_stack *b);
void				op_rrr(t_stack *a, t_stack *b);

// algos/index_assign.c
void				rank_normalize(t_stack *a);

// algos/tiny_sort.c
void				small_block_sort(t_stack *a, t_stack *b);

// algos/sort_radix.c
void				sort_radix(t_stack *a, t_stack *b);

// utils/errors.c
int					ps_error(void);

#endif // PS_H
