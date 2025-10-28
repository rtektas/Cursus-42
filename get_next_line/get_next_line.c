/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 17:40:18 by rtektas           #+#    #+#             */
/*   Updated: 2024/06/12 19:39:54 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static t_list	*stash = NULL;
	char			*line;

	if (BUFFER_SIZE >= 2147483647)
		return (NULL);
	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		line = NULL;
	}
	read_and_stash(fd, &stash);
	if (stash == NULL)
		return (NULL);
	extract_line(stash, &line);
	clean_stash(&stash);
	if (line[0] == '\0')
	{
		free_stash(stash);
		stash = NULL;
		free(line);
		return (NULL);
	}
	return (line);
}

/* Utilise read() pour ajouter des caractères au stash */

void	read_and_stash(int fd, t_list **stash)
{
	char	*buf;
	int		nb_caractere_lu;

	nb_caractere_lu = 1;
	while (!found_newline(*stash) && nb_caractere_lu != 0)
	{
		buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (buf == NULL)
			return ;
		nb_caractere_lu = (int)read(fd, buf, BUFFER_SIZE);
		if ((*stash == NULL && nb_caractere_lu == 0) || nb_caractere_lu == -1)
		{
			free(buf);
			if (nb_caractere_lu == -1 && stash)
				free_stash(*stash);
			*stash = NULL;
			return ;
		}
		buf[nb_caractere_lu] = '\0';
		add_to_stash(stash, buf, nb_caractere_lu);
		free(buf);
	}
}

/* Ajoute le contenu de notre buffer à la fin de notre stash */

void	add_to_stash(t_list **stash, char *buf, int nb_caractere_lu)
{
	int		i;
	t_list	*last;
	t_list	*new_node;

	new_node = malloc(sizeof(t_list));
	if (new_node == NULL)
		return ;
	new_node->next = NULL;
	new_node->content = malloc(sizeof(char) * (nb_caractere_lu + 1));
	if (new_node->content == NULL)
		return ;
	i = 0;
	while (buf[i] && i < nb_caractere_lu)
	{
		new_node->content[i] = buf[i];
		i++;
	}
	new_node->content[i] = '\0';
	if (*stash == NULL)
	{
		*stash = new_node;
		return ;
	}
	last = ft_lst_get_last(*stash);
	last->next = new_node;
}

/* Extrait tous les caractères de notre stash et les stocke dans notre ligne.
 * S'arrête après le premier \n rencontré */

void	extract_line(t_list *stash, char **line)
{
	int	i;
	int	j;

	if (stash == NULL)
		return ;
	generate_line(line, stash);
	if (*line == NULL)
		return ;
	j = 0;
	while (stash)
	{
		i = 0;
		while (stash->content[i])
		{
			if (stash->content[i] == '\n')
			{
				(*line)[j++] = stash->content[i];
				break ;
			}
			(*line)[j++] = stash->content[i++];
		}
		stash = stash->next;
	}
	(*line)[j] = '\0';
}

/* Après avoir extrait la ligne qui a été lue,
	nous n'avons plus besoin de ces caractères
Cette fonction nettoie le stash afin que seuls les
caractères non retournés à la fin de get_next_line
 * restent dans notre stash statique. */

void	clean_stash(t_list **stash)
{
	t_list	*last;
	t_list	*clean_node;
	int		i;
	int		j;

	clean_node = malloc(sizeof(t_list));
	if (stash == NULL || clean_node == NULL)
		return ;
	clean_node->next = NULL;
	last = ft_lst_get_last(*stash);
	i = 0;
	while (last->content[i] && last->content[i] != '\n')
		i++;
	if (last->content && last->content[i] == '\n')
		i++;
	clean_node->content = malloc(sizeof(char) * ((ft_strlen(last->content) - i)
				+ 1));
	if (clean_node->content == NULL)
		return ;
	j = 0;
	while (last->content[i])
		clean_node->content[j++] = last->content[i++];
	clean_node->content[j] = '\0';
	free_stash(*stash);
	*stash = clean_node;
}
