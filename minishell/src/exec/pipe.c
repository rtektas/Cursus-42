/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 11:27:56 by asaracut          #+#    #+#             */
/*   Updated: 2025/10/24 18:26:43 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	wait_children_set_status(t_minishell **ms)
{
	int	status;

	status = 0;
	while (wait(&status) > 0)
		;
	if (WIFEXITED(status))
		(*ms)->last_status = WEXITSTATUS(status);
	else
		(*ms)->last_status = 1;
}

int	update_pipe(int fd[2], int i, int n)
{
	if (i < n - 1)
		return (fd[0]);
	return (-1);
}

void	exec_one_child(t_exec_args args)
{
	fork_child(args.cmd, args.paths, args.ms,
		(int [2]){args.prev_in, args.fd[1]});
	if (args.prev_in != -1)
		close(args.prev_in);
	if (args.fd[1] != -1)
		close(args.fd[1]);
}

int	handle_pipe(int fd[2], int i, int n, t_minishell **ms)
{
	if (i < n - 1)
	{
		if (pipe(fd) == -1)
		{
			(*ms)->last_status = 1;
			return (0);
		}
	}
	else
		fd[1] = -1;
	return (1);
}

int	exec_pipeline(t_command *cmd, t_minishell **ms, char **paths, int n)
{
	t_exec_args	args;
	int			fd[2];
	int			prev_in;
	int			i;

	i = 0;
	prev_in = -1;
	while (i < n && cmd)
	{
		if (handle_pipe(fd, i, n, ms) == 0)
			return (1);
		args.cmd = cmd;
		args.paths = paths;
		args.ms = ms;
		args.fd[0] = fd[0];
		args.fd[1] = fd[1];
		args.prev_in = prev_in;
		exec_one_child(args);
		prev_in = update_pipe(fd, i, n);
		cmd = cmd->next;
		i++;
	}
	wait_children_set_status(ms);
	return ((*ms)->last_status);
}
