/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 00:00:00 by rtektas           #+#    #+#             */
/*   Updated: 2025/11/14 00:00:00 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

size_t	count_lines(const char *s)
{
	size_t	i;
	size_t	lines;

	i = 0;
	lines = 1;
	while (s[i])
	{
		if (s[i] == '\n')
			lines++;
		i++;
	}
	return (lines);
}

char	*dup_seg(const char *s, size_t start, size_t len)
{
	char	*row;
	size_t	k;

	row = (char *)malloc(len + 1);
	if (!row)
		return (NULL);
	k = 0;
	while (k < len)
	{
		row[k] = s[start + k];
		k++;
	}
	if (len > 0 && row[len - 1] == '\r')
		len--;
	row[len] = '\0';
	return (row);
}

void	free_rows(char **arr, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
		free(arr[i++]);
	free(arr);
}

char	**dup_map(char **src, int h, int w)
{
	char	**dst;
	int		i;
	int		j;

	dst = (char **)malloc(sizeof(char *) * h);
	if (!dst)
		return (NULL);
	i = 0;
	while (i < h)
	{
		dst[i] = (char *)malloc(w + 1);
		if (!dst[i])
			return (NULL);
		j = 0;
		while (j < w)
		{
			dst[i][j] = src[i][j];
			j++;
		}
		dst[i][w] = '\0';
		i++;
	}
	return (dst);
}

void	free_map(char **m, int h)
{
	int	i;

	if (!m)
		return ;
	i = 0;
	while (i < h)
	{
		free(m[i]);
		i++;
	}
	free(m);
}
