#include "ft_printf.h"

/*
** Converts a signed integer into decimal representation [-]dddd.
*/

int		pf_cv_di(t_modifier *m, t_array *d, va_list ap)
{
	return (pf_signed_integer(m, d, ap, 10));
}

/*
** Converts an unsigned integer into decimal representation dddd.
*/

int		pf_cv_u(t_modifier *m, t_array *d, va_list ap)
{
	return (pf_unsigned_integer(m, d, ap, 10));
}

/*
** Converts an unsigned integer into hexadecimal representation hhhh.
*/

int		pf_cv_x(t_modifier *m, t_array *d, va_list ap)
{
	if (m->booleans.n.alternate)
		fta_append(d, "0x", 2);
	return (pf_unsigned_integer(m, d, ap, 16));
}

/*
** Converts an unsigned integer into hexadecimal representation HHHH.
*/

int		pf_cv_cx(t_modifier *m, t_array *d, va_list ap)
{
	if (m->booleans.n.alternate)
		fta_append(d, "0X", 2);
	return (pf_unsigned_integer(m, d, ap, -16));
}

/*
** Writes an implementation defined character sequence defining a pointer.
*/

int		pf_cv_p(t_modifier *m, t_array *d, va_list ap)
{
	(void)m;
	fta_append(d, "0x", 2);
	m->length = 'L';
	return (pf_unsigned_integer(m, d, ap, 16));
}

/*
** Converts a unsigned integer into octal representation oooo.
*/

int		pf_cv_o(t_modifier *m, t_array *d, va_list ap)
{
	int alt;

	alt = 0;
	if (m->booleans.n.alternate)
	{
		fta_append(d, "0", 1);
		alt = 1;
	}
	return (pf_unsigned_integer(m, d, ap, 8) + alt);
}

/*
** Converts a unsigned integer into binary representation bbbb.
*/

int		pf_cv_b(t_modifier *m, t_array *d, va_list ap)
{
	if (m->booleans.n.alternate)
		fta_append(d, "b", 1);
	return (pf_unsigned_integer(m, d, ap, 2));
}

/*
** returns the number of characters written so far by this call to the function.
*/

int		pf_cv_n(t_modifier *m, t_array *d, va_list ap)
{
	int		*arg;

	arg = va_arg(ap, void *);
	if (!arg)
		return(-1);
	m->size = 0;
	m->precision = -1;
	if (m->length == 'H')
		*(char *)arg = (char)d->size;
	else if (m->length == 'h')
		*(short *)arg = (short)d->size;
	else if (m->length == 'l' || m->length == 'z')
		*(long *)arg = (long)d->size;
	else if (m->length == 'L')
		*(long long *)arg = (long long)d->size;
	else if (m->length == 'j')
		*(intmax_t *)arg = (intmax_t)d->size;
	else
		*(int *)arg = (int)d->size;
	return (0);
}

/*
** pf_itoa_base appends _n_ expressed in base _b_ in _d_
** if the weakest bit of _info_ is on, capital letters will be used
** |  for digits greater than 10.
** if the second weakest bit of _info_ is on, _n_ is considered unsigned.
*/

int			pf_itoa_base(t_array *d, intmax_t n, int b, char info)
{
	int					ans;
	const uintmax_t		un = (uintmax_t)n;
	const char *const	base =

	(info & 1 ? "0123456789ABCDEF" : "0123456789abcdef");
	ans = 1;
	if (info & 2 ? (uintmax_t)b <= un : n <= -b || b <= n)
		ans += pf_itoa_base(d, (info & 2 ?
			(intmax_t)(un / b) : n / b), b, info);
	fta_append(d, (void *)(base +
		(info & 2 ? (size_t)(un % b) : ABS(n % b))), 1);
	return (ans);
}

int			pf_signed_integer(t_modifier *m, t_array *d, va_list ap, int b)
{
	intmax_t	arg;

	if (m->length == 'H')
		arg = (signed char)va_arg(ap, int);
	else if (m->length == 'h')
		arg = (short)va_arg(ap, int);
	else if (m->length == 'l' || m->length == 'z')
		arg = va_arg(ap, long);
	else if (m->length == 'L')
		arg = va_arg(ap, long long);
	else if (m->length == 'j')
		arg = va_arg(ap, intmax_t);
	else
		arg = va_arg(ap, int);
	if (arg < 0)
		fta_append(d, "-", 1);
	else if (m->booleans.n.plus)
		fta_append(d, "+", 1);
	else if (m->booleans.n.space)
		fta_append(d, " ", 1);
	if (arg == 0 && m->precision == 0)
		return (0);
	return (pf_itoa_base(d, arg, ABS(b), b < 0));
}

int			pf_unsigned_integer(t_modifier *m, t_array *d, va_list ap, int b)
{
	uintmax_t	arg;

	if (m->length == 'H')
		arg = (unsigned char)va_arg(ap, unsigned);
	else if (m->length == 'h')
		arg = (unsigned short)va_arg(ap, unsigned);
	else if (m->length == 'l')
		arg = va_arg(ap, unsigned long);
	else if (m->length == 'L')
		arg = va_arg(ap, unsigned long long);
	else if (m->length == 'z')
		arg = va_arg(ap, size_t);
	else if (m->length == 'j')
		arg = va_arg(ap, uintmax_t);
	else
		arg = va_arg(ap, unsigned);
	if (arg == 0 && m->booleans.n.alternate
		&& (m->conversion == 'x' || m->conversion == 'X'))
		d->size -= 2;
	if (arg == 0 && m->precision == 0)
		return (0);
	if (arg == 0 && m->conversion == 'o' && m->booleans.n.alternate)
		return (0);
	return (pf_itoa_base(d, arg, ABS(b), 2 | (b < 0)));
}
