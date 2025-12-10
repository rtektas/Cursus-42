/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 17:37:51 by rtektas           #+#    #+#             */
/*   Updated: 2025/06/11 17:37:51 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	expand_args(char **argv, char **env, int last_status, int type)
{
	int		i;
	char	*expanded;

	i = 0;
	while (argv[i])
	{
		expanded = expand(argv[i], env, last_status, type);
		free(argv[i]);
		argv[i] = expanded;
		i++;
	}
}

static void	handle_regular_char(const char *input, t_expand_ctx *ctx)
{
	ctx->out[(*ctx->oi)++] = input[(*ctx->i)++];
}

static void	expand_loop(const char *input, t_expand_ctx *ctx)
{
	while (input[*(ctx->i)])
	{
		if (input[*(ctx->i)] == '"' || input[*(ctx->i)] == '\'')
			handle_quotes(input, ctx);
		else if (input[*(ctx->i)] == '$')
		{
			if (handle_dollar(input, ctx))
				continue ;
			handle_regular_char(input, ctx);
		}
		else
			handle_regular_char(input, ctx);
	}
	ctx->out[*ctx->oi] = '\0';
}

char	*expand(const char *input, char **env, int last_status, int type)
{
	char			*out;
	int				oi;
	int				i;
	t_expand_ctx	ctx;

	out = malloc(4096);
	if (!out)
		return (NULL);
	oi = 0;
	i = 0;
	ctx.out = out;
	ctx.oi = &oi;
	ctx.env = env;
	ctx.last_status = last_status;
	ctx.i = &i;
	ctx.type = type;
	expand_loop(input, &ctx);
	return (out);
}
