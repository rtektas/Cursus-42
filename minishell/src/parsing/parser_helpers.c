/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 20:40:00 by marvin            #+#    #+#             */
/*   Updated: 2025/11/07 20:40:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	placeholder_replace_argv0(t_command *cmd, const char *val)
{
	free(cmd->argv[0]);
	cmd->argv[0] = ft_strdup(val);
	if (!cmd->argv[0])
		return (-1);
	cmd->argv[1] = NULL;
	return (0);
}

static int	ensure_capacity(t_command *cmd)
{
	char	**n_av;
	int		n_size;

	if (cmd->argc + 1 < cmd->size)
		return (0);
	n_size = cmd->size * 2;
	n_av = malloc(sizeof(char *) * n_size);
	if (!n_av)
		return (-1);
	ft_memcpy(n_av, cmd->argv, sizeof(char *) * cmd->argc);
	free(cmd->argv);
	cmd->argv = n_av;
	cmd->size = n_size;
	return (0);
}

static int	append_arg(t_command *cmd, const char *val)
{
	if (ensure_capacity(cmd) < 0)
		return (-1);
	cmd->argv[cmd->argc] = ft_strdup(val);
	if (!cmd->argv[cmd->argc])
		return (-1);
	cmd->argc++;
	cmd->argv[cmd->argc] = NULL;
	return (0);
}

int	add_arg(t_command *cmd, const char *val)
{
	if (cmd->argv && cmd->argc >= 1 && cmd->argv[0] && cmd->argv[0][0] == '\0')
		return (placeholder_replace_argv0(cmd, val));
	return (append_arg(cmd, val));
}
