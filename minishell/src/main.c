/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 03:13:18 by asaracut          #+#    #+#             */
/*   Updated: 2025/11/11 15:23:46 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_minishell	*ft_init_minishell(int ac, char **av, char **envp)
{
	t_minishell	*minishell;

	(void)ac;
	(void)av;
	minishell = malloc(sizeof(t_minishell));
	if (!minishell)
		return (NULL);
	ft_strdup_tab(&minishell, envp);
	if (!minishell->envp)
	{
		free(minishell);
		return (NULL);
	}
	minishell->token = NULL;
	minishell->h_history = NULL;
	minishell->cmd = -1;
	bump_shlvl(minishell);
	return (minishell);
}

void	cleanup_minishell(t_minishell *minishell)
{
	if (!minishell)
		return ;
	if (minishell->envp)
		ft_free_tab(minishell->envp);
	if (minishell->token)
		free_token_list((t_token *)minishell->token);
	free(minishell);
}

void	minishell_loop(t_minishell **minishell)
{
	char	*line;
	t_token	*tokens;

	while (1)
	{
		signal(SIGINT, sig_handler);
		signal(SIGQUIT, SIG_IGN);
		line = read_full_command();
		if (!line)
			break ;
		if (*line)
			add_history(line);
		tokens = lexer(line);
		if (!tokens && *line)
			minishell[0]->last_status = 2;
		parse_token(tokens, minishell);
		free_token_list(tokens);
		free(line);
	}
	return ;
}

int	main(int ac, char **av, char **envp)
{
	t_minishell	*minishell;

	minishell = ft_init_minishell(ac, av, envp);
	if (!minishell)
		return (1);
	minishell_loop(&minishell);
	cleanup_minishell(minishell);
	return (0);
}
