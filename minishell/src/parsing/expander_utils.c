/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 17:37:51 by rtektas           #+#    #+#             */
/*   Updated: 2025/06/11 17:37:51 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_env_value(const char *name, char **env)
{
	int	len;
	int	i;

	if (!name || !env)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

int	handle_dollar_var(const char *input, t_expand_ctx *ctx)
{
	int		start;
	int		len;
	char	var[256];
	char	*val;

	start = *(ctx->i) + 1;
	len = 0;
	while (ft_isalnum(input[start + len]) || input[start + len] == '_')
		len++;
	ft_strlcpy(var, input + start, len + 1);
	val = get_env_value(var, ctx->env);
	if (!val)
		val = "";
	ft_strlcpy(ctx->out + *(ctx->oi), val, ft_strlen(val) + 1);
	*(ctx->oi) += ft_strlen(val);
	*(ctx->i) = start + len;
	return (1);
}

int	handle_dollar(const char *input, t_expand_ctx *ctx)
{
	char	*num;
	size_t	len;

	if (input[*(ctx->i) + 1] == '?')
	{
		num = ft_itoa(ctx->last_status);
		if (num)
		{
			len = ft_strlen(num);
			ft_strlcpy(ctx->out + *(ctx->oi), num, len + 1);
			*(ctx->oi) += len;
			free(num);
		}
		*(ctx->i) += 2;
		return (1);
	}
	if (ft_isalpha(input[*(ctx->i) + 1]) || input[*(ctx->i) + 1] == '_')
		return (handle_dollar_var(input, ctx));
	return (0);
}

void	handle_quotes(const char *input, t_expand_ctx *ctx)
{
	char	quote;

	quote = input[*(ctx->i)];
	(*(ctx->i))++;
	while (input[*(ctx->i)] && input[*(ctx->i)] != quote)
	{
		if (quote == '"' && input[*(ctx->i)] == '$')
		{
			if (handle_dollar(input, ctx))
				continue ;
		}
		ctx->out[(*(ctx->oi))++] = input[(*(ctx->i))++];
	}
	if (input[*(ctx->i)] == quote)
		(*(ctx->i))++;
}
