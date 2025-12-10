/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 10:15:00 by marvin            #+#    #+#             */
/*   Updated: 2025/11/07 10:15:00 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	run_builtin_with_redir(t_command *cmd, t_minishell **ms)
{
	int	save_in;
	int	save_out;
	int	status;

	save_in = dup(STDIN_FILENO);
	save_out = dup(STDOUT_FILENO);
	if (save_in == -1 || save_out == -1)
		return (perror("dup"), 1);
	if (apply_redirections(cmd))
		status = 1;
	else
		status = exec_builtin(cmd->argv, &(*ms)->envp);
	dup2(save_in, STDIN_FILENO);
	dup2(save_out, STDOUT_FILENO);
	close(save_in);
	close(save_out);
	return (status);
}

int	exec_only_redirections(t_command *cmd, t_minishell **ms)
{
	int	save_in;
	int	save_out;
	int	st;

	save_in = dup(STDIN_FILENO);
	save_out = dup(STDOUT_FILENO);
	st = 0;
	if (save_in == -1 || save_out == -1)
		return (perror("dup"), (*ms)->last_status = 1);
	if (apply_redirections(cmd))
		st = 1;
	dup2(save_in, STDIN_FILENO);
	dup2(save_out, STDOUT_FILENO);
	close(save_in);
	close(save_out);
	(*ms)->last_status = st;
	return ((*ms)->last_status);
}
