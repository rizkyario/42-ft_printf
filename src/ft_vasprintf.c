/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vasprintf.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rnugroho <rnugroho@students.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/03 18:36:35 by rnugroho          #+#    #+#             */
/*   Updated: 2018/02/25 01:13:47 by rnugroho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static const char
	*pf_update_value(char const *s, int *v, va_list dap)
{
	if (*s == '*')
	{
		*v = va_arg(dap, unsigned);
		return (s + 1);
	}
	*v = 0;
	while ('0' <= *s && *s <= '9')
		*v = 10 * (*v) + *s++ - '0';
	return (s);
}

static void
	pf_set_length_modifier(char c, char *lm)
{
	if (c == 'h' && *lm == 'h')
		*lm = ft_toupper(c);
	else if (c == 'l' && *lm == 'l')
		*lm = '7';
	else if (*lm == 0 || !(c == 'h' && *lm != 'H'))
		*lm = c;
}

static const char
	*pf_match(char const *s, t_modifier *m, va_list ap, va_list dap)
{
	int n;
	int	temp;

	*m = NEW_MODIFIER;
	while (*s != '\0')
	{
		if (*s == '.')
		{
			s = pf_update_value(s + 1, &(m->precision), dap) - 1;
			if (m->dollar)
				va_copy(ap, dap);
		}
		else if (*s == '\'')
			m->quote = 1;
		else if (*s == '$')
		{
			if (*(s - 1) != '*')
			{
				m->ndollar = m->size;
				m->size = 0;
			}
			m->dollar = 1;
		}
		else if (('1' <= *s && *s <= '9') || *s == '*')
		{
			if (m->size == 0)
				s = pf_update_value(s, &(m->size), dap) - 1;
			if (m->dollar)
				va_copy(ap, dap);
			if (*(s - 1) == '*')
			{
				temp = m->size;
				m->size = m->ndollar;
				m->ndollar = temp;
			}
		}
		else if ((n = is_in(*s, FTPF_SWITCHES)) >= 0)
			m->booleans.t[n] = 1;
		else if (is_in(*s, FTPF_LM) >= 0)
			pf_set_length_modifier(*s, &(m->length));
		else if ((m->conversion = *s))
			return (s + 1);
		s++;
	}
	return (s);
}

static const char
	*pf_next_specifier(char const *s, t_array *d)
{
	char const	*p;

	p = s;
	while (*p != '\0' && *p != '%')
		p++;
	if (p != s)
		fta_append(d, (void *)s, p - s);
	return (p);
}

int
	ft_vasprintf(char **ret, char const *s, va_list ap)
{
	t_array		d;
	t_modifier	m;
	int			temp;
	va_list		dap;

	va_copy(dap, ap);
	d = NEW_ARRAY(char);
	fta_reserve(&d, ft_strlen(s));
	while (*s != '\0')
	{
		if (*s == '%')
		{
			s = pf_match(s + 1, &m, ap, dap);
			if (m.conversion && pf_convert(&m, &d, ap, dap) == -1)
			{
				fta_resize(&d, temp);
				fta_trim(&d);
				*ret = d.data;
				return (-1);
			}
			temp = d.size;
		}
		s = pf_next_specifier(s, &d);
	}
	fta_trim(&d);
	*ret = d.data;
	return (d.size - 1);
}
