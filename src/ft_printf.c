/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnugroho <rnugroho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/03 18:35:59 by rnugroho          #+#    #+#             */
/*   Updated: 2018/02/27 14:37:18 by rnugroho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/*
** _format_ is the string describing the expected output,
** the data being given through a unknown number
** of parameters following _format_.
** All those functions return the number of bytes written.
*/

/*
** Writes on the standard output.
*/

int
	ft_printf(char const *format, ...)
{
	va_list		ap;
	int			ans;

	va_start(ap, format);
	ans = ft_vprintf(format, ap);
	va_end(ap);
	return (ans);
}

/*
** Writes on the standard output ends with \n.
*/

int
	ft_printfln(char const *format, ...)
{
	va_list		ap;
	int			ans;

	va_start(ap, format);
	ans = ft_vprintf(format, ap);
	va_end(ap);
	ft_printf("\n");
	return (ans + 1);
}

/*
** Writes on the specified fd.
*/

int
	ft_dprintf(int fd, char const *format, ...)
{
	va_list		ap;
	int			ans;

	va_start(ap, format);
	ans = ft_vdprintf(fd, format, ap);
	va_end(ap);
	return (ans);
}

/*
** Allocates a string, writes into it, and returns it through _ret_
*/

int
	ft_asprintf(char **ret, char const *format, ...)
{
	va_list		ap;
	va_list		dap;
	int			ans;

	va_start(ap, format);
	va_copy(dap, ap);
	ans = ft_vasprintf(ret, format, ap, dap);
	va_end(ap);
	return (ans);
}

/*
** Allocates a string, and returns it
*/

char
	*ft_rasprintf(int *ans, char const *format, ...)
{
	va_list		ap;
	va_list		dap;
	char		*ret;

	va_start(ap, format);
	va_copy(dap, ap);
	*ans = ft_vasprintf(&ret, format, ap, dap);
	va_end(ap);
	return (ret);
}
