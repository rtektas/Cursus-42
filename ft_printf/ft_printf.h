/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <resultektas.idb@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 16:47:52 by rtektas           #+#    #+#             */
/*   Updated: 2024/05/03 16:47:55 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>

void	ft_put_hex(unsigned int num, const char format);
void	ft_put_pointeur(unsigned long long nb);
int		ft_len(int n);
int		ft_len_hex(unsigned int num);
int		ft_len_pointeur(unsigned long long nb);
int		ft_unsigned_len(unsigned int nb);
int		ft_putchar(int c);
int		ft_putstr(char *str);
int		ft_putnbr(int n);
int		ft_put_hex_final(unsigned int num, const char format);
int		ft_put_pointeur_final(unsigned long long ptr);
int		ft_putunsigned(unsigned int nb);
int		ft_conversion(va_list vl, const char format);
int		ft_printf(const char *format, ...);
char	*ft_char(char *s, unsigned int number, int len);
char	*ft_itoa(int n);
char	*ft_unsigned_itoa(unsigned int nb);

#endif
