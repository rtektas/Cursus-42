/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 20:28:55 by marvin            #+#    #+#             */
/*   Updated: 2025/11/07 20:28:55 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	needs_more_input(const char *s)
{
	int	in_s;
	int	in_d;
	int	i;

	in_s = 0;
	in_d = 0;
	i = 0;
	while (s && s[i])
	{
		if (s[i] == '\'' && !in_d)
			in_s = !in_s;
		else if (s[i] == '"' && !in_s)
			in_d = !in_d;
		i++;
	}
	return (in_s || in_d);
}

static char	*return_empty_after_interrupt(void)
{
	g_signal = 0;
	return (ft_strdup(""));
}

static int	append_with_newline(char **buf, const char *cont)
{
	char	*tmp;
	char	*joined;

	tmp = ft_strjoin(*buf, "\n");
	free(*buf);
	if (!tmp)
		return (1);
	joined = ft_strjoin(tmp, cont);
	free(tmp);
	if (!joined)
		return (1);
	*buf = joined;
	return (0);
}

static int	read_continuations(char **buf)
{
	char	*cont;

	while (needs_more_input(*buf))
	{
		if (g_signal == SIGINT)
			return (free(*buf), *buf = NULL, g_signal = 0, 1);
		cont = readline("> ");
		if (!cont)
			return (free(*buf), *buf = NULL, 1);
		if (g_signal == SIGINT)
			return (free(cont), free(*buf), *buf = NULL, g_signal = 0, 1);
		if (append_with_newline(buf, cont))
			return (free(cont), free(*buf), *buf = NULL, 1);
		free(cont);
	}
	return (0);
}

char	*read_full_command(void)
{
	char	*line;
	char	*buf;

	if (g_signal == SIGINT)
		return (return_empty_after_interrupt());
	line = readline("minishell> ");
	if (!line)
		return (NULL);
	buf = ft_strdup(line);
	free(line);
	if (!buf)
		return (NULL);
	if (g_signal == SIGINT)
		return (free(buf), return_empty_after_interrupt());
	if (read_continuations(&buf))
		return (buf);
	return (buf);
}
