/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 13:29:16 by marvin            #+#    #+#             */
/*   Updated: 2025/11/14 13:29:16 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ps.h"

/* helper: consume digit sequence, check overflow */
static int	consume_digits(const char *s, int *i, long long *acc,
		long long sign)
{
	while (ft_isdigit((unsigned char)s[*i]))
	{
		*acc = (*acc * 10) + (s[*i] - '0');
		if ((sign == 1 && *acc > INT_MAX) || (sign == -1 && - *acc < INT_MIN))
			return (-1);
		(*i)++;
	}
	return (0);
}

/* parse strict int with overflow & format checks */
static int	str_to_int_strict(const char *s, int *out)
{
	long long	acc;
	long long	sign;
	int			idx;

	acc = 0;
	sign = 1;
	idx = 0;
	while (s[idx] == ' ' || (s[idx] >= 9 && s[idx] <= 13))
		idx++;
	if (s[idx] == '+' || s[idx] == '-')
	{
		if (s[idx] == '-')
			sign = -1;
		idx++;
	}
	if (!ft_isdigit((unsigned char)s[idx]))
		return (-1);
	if (consume_digits(s, &idx, &acc, sign) != 0)
		return (-1);
	while (s[idx] == ' ' || (s[idx] >= 9 && s[idx] <= 13))
		idx++;
	if (s[idx] != '\0')
		return (-1);
	*out = (int)(sign * acc);
	return (0);
}

/* duplicate check (<=25 lines) */
static int	value_exists(const t_stack *a, int val)
{
	t_node	*cur;

	cur = a->head;
	while (cur)
	{
		if (cur->val == val)
			return (1);
		cur = cur->next;
	}
	return (0);
}

/* split one argv token by spaces and append values preserving order */
static int	consume_token_into_stack(t_stack *a, const char *arg)
{
	char	**parts;
	int		v;
	int		i;
	int		rc;

	parts = ft_split(arg, ' ');
	if (!parts)
		return (-1);
	v = 0;
	i = 0;
	rc = 0;
	while (parts[i] && rc == 0)
	{
		if (parts[i][0] == '\0' || str_to_int_strict(parts[i], &v) != 0
			|| value_exists(a, v))
			rc = -1;
		else
			deque_push_tail(a, v);
		i++;
	}
	i = 0;
	while (parts[i])
		free(parts[i++]);
	free(parts);
	return (rc);
}

int	ps_parse_args_build(t_stack *a, int argc, char **argv)
{
	int	idx;

	idx = 1;
	while (idx < argc)
	{
		if (consume_token_into_stack(a, argv[idx]) != 0)
			return (-1);
		idx++;
	}
	return (0);
}
