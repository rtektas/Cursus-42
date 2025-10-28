/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 20:22:01 by rtektas           #+#    #+#             */
/*   Updated: 2024/04/22 18:14:49 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	compter_mots(char const *s, char c)
{
	size_t	compte;
	size_t	indice;

	compte = 0;
	indice = 0;
	while (*(s + indice))
	{
		if (*(s + indice) != c)
		{
			compte++;
			while (*(s + indice) && *(s + indice) != c)
				indice++;
		}
		else if (*(s + indice) == c)
			indice++;
	}
	return (compte);
}

static size_t	obtenir_longueur_mot(char const *s, char c)
{
	size_t	indice;

	indice = 0;
	while (*(s + indice) && *(s + indice) != c)
		indice++;
	return (indice);
}

static void	liberer_tableau(size_t i, char **tableau)
{
	while (i > 0)
	{
		i--;
		free(*(tableau + i));
	}
	free(tableau);
}

static char	**diviser(char const *s, char c, char **tableau, size_t compte_mots)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (i < compte_mots)
	{
		while (*(s + j) && *(s + j) == c)
			j++;
		*(tableau + i) = ft_substr(s, j, obtenir_longueur_mot(&*(s + j), c));
		if (!*(tableau + i))
		{
			liberer_tableau(i, tableau);
			return (NULL);
		}
		while (*(s + j) && *(s + j) != c)
			j++;
		i++;
	}
	*(tableau + i) = NULL;
	return (tableau);
}

char	**ft_split(char const *s, char c)
{
	char	**array;
	size_t	words;

	if (!s)
		return (NULL);
	words = compter_mots(s, c);
	array = (char **)malloc(sizeof(char *) * (words + 1));
	if (!array)
		return (NULL);
	array = diviser(s, c, array, words);
	return (array);
}
