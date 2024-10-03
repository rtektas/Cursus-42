/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 17:48:38 by rtektas           #+#    #+#             */
/*   Updated: 2024/10/03 17:48:39 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 30
# endif

char	*get_next_line(int fd);
char	*f_strchr(const char *str, int c);
char	*f_strjoin(char *s1, char *s2);
size_t	f_strlen(const char *str);
char	*f_strdup(const char *s1);
char	*f_substr(char *s, unsigned int start, size_t len);
char	*strappend(char **str, char *str2);
char	*get_data(char **backup, int fd);
#endif