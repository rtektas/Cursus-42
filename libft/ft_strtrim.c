/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 20:13:07 by rtektas           #+#    #+#             */
/*   Updated: 2024/04/19 19:54:04 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	verif_char(char const *chaine, char const c)
{
	size_t	indice;

	if (!chaine)
		return (0);
	indice = 0;
	while (*(chaine + indice))
	{
		if (*(chaine + indice) == c)
			return (1);
		indice++;
	}
	return (0);
}

static char	*str_new(size_t n)
{
	char	*str;

	str = (char *)malloc(sizeof(char) * (n + 1));
	if (!str)
		return (NULL);
	return (str);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*chaine_trim;
	size_t	debut;
	size_t	fin;
	size_t	indice;

	if (!s1 || !set)
		return (NULL);
	debut = 0;
	while (*(s1 + debut) && verif_char(set, *(s1 + debut)))
		debut++;
	fin = ft_strlen(s1);
	while (fin > debut && verif_char(set, *(s1 + (fin - 1))))
		fin--;
	chaine_trim = str_new(fin - debut);
	if (!chaine_trim)
		return (NULL);
	indice = 0;
	while ((debut + indice) < fin)
	{
		*(chaine_trim + indice) = *(s1 + (debut + indice));
		indice++;
	}
	*(chaine_trim + indice) = '\0';
	return (chaine_trim);
}
