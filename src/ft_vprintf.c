/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnugroho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/03 18:36:14 by rnugroho          #+#    #+#             */
/*   Updated: 2017/12/03 18:36:15 by rnugroho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		ft_vprintf(char const *format, va_list ap)
{
	return (ft_vdprintf(1, format, ap));
}

int		ft_vdprintf(int fd, char const *format, va_list ap)
{
	char		*str;
	int			ans;

	ans = ft_vasprintf(&str, format, ap);
	write(fd, str, ans);
	free(str);
	return (ans);
}
