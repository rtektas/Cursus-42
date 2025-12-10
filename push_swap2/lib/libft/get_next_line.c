/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asaracut <asaracut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 22:37:02 by asaracut          #+#    #+#             */
/*   Updated: 2025/10/24 18:26:43 by asaracut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

char	*ft_reserve(char *reserve, int fd)
{
	char	*buffer;
	int		read_bytes;

	read_bytes = 1;
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (ft_free_gnl(reserve));
	while (read_bytes > 0)
	{
		read_bytes = read(fd, buffer, BUFFER_SIZE);
		if (read_bytes == -1)
			return (ft_free_gnl(buffer), ft_free_gnl(reserve));
		buffer[read_bytes] = '\0';
		reserve = ft_strjoin_gnl(reserve, buffer);
		if (!reserve || ft_strchr_gnl(buffer, '\n'))
			break ;
	}
	ft_free_gnl(buffer);
	return (reserve);
}

char	*ft_result(char *reserve)
{
	char	*result;
	int		i;

	i = 0;
	if (!reserve || !reserve[i])
		return (NULL);
	while (reserve[i] && reserve[i] != '\n')
		i++;
	result = malloc(i + 2);
	if (!result)
		return (NULL);
	i = 0;
	while (reserve[i] && reserve[i] != '\n')
	{
		result[i] = reserve[i];
		i++;
	}
	if (reserve[i] == '\n')
		result[i++] = '\n';
	result[i] = '\0';
	return (result);
}

char	*ft_new_reserve(char *reserve)
{
	int		i;
	int		j;
	char	*new_res;

	i = 0;
	while (reserve[i] && reserve[i] != '\n')
		i++;
	if (!reserve[i])
		return (ft_free_gnl(reserve));
	new_res = malloc(ft_strlen(reserve) - i + 1);
	if (!new_res)
		return (ft_free_gnl(reserve));
	i++;
	j = 0;
	while (reserve[i])
		new_res[j++] = reserve[i++];
	new_res[j] = '\0';
	ft_free_gnl(reserve);
	return (new_res);
}

char	*get_next_line(int fd)
{
	static char	*reserve;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	reserve = ft_reserve(reserve, fd);
	if (!reserve)
		return (NULL);
	line = ft_result(reserve);
	reserve = ft_new_reserve(reserve);
	return (line);
}
