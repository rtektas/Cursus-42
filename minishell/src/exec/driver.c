/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   driver.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 16:45:20 by marvin            #+#    #+#             */
/*   Updated: 2025/11/05 16:45:20 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	expand_all_args(t_command *cmd, t_minishell **ms)
{
	while (cmd)
	{
		expand_args(cmd->argv, (*ms)->envp, (*ms)->last_status, cmd->type);
		cmd = cmd->next;
	}
}

static void	run_commands(t_command *cmd, t_minishell **ms, char **paths)
{
	int	nb_cmds;

	nb_cmds = count_cmds(cmd);
	if (nb_cmds == 1)
	{
		exec_single(cmd, ms, paths);
		(*ms)->last_status = (*ms)->last_status;
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		exec_pipeline(cmd, ms, paths, nb_cmds);
		signal(SIGINT, sig_handler);
		signal(SIGQUIT, SIG_IGN);
		(*ms)->last_status = (*ms)->last_status;
	}
}

void	handle_exec(t_command *cmd, t_minishell **ms)
{
	char	**paths;

	paths = ft_find_path((*ms)->envp);
	if (prepare_heredocs(cmd, ms))
	{
		if (paths)
			ft_free_tab(paths);
		return ;
	}
	if (paths)
		ft_add_slash(paths);
	expand_all_args(cmd, ms);
	run_commands(cmd, ms, paths);
	if (paths)
		ft_free_tab(paths);
}
