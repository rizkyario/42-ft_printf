/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angagnie <angagnie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/01 19:15:32 by angagnie          #+#    #+#             */
/*   Updated: 2017/09/22 22:23:39 by angagnie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

#include <stdio.h>
#include <string.h>

#define TEST(FMT,...)													\
	pf = asprintf(&pf_dst,FMT,##__VA_ARGS__);							\
	ft = ft_asprintf(&ft_dst,FMT,##__VA_ARGS__);						\
	if (ft != pf)														\
		dprintf(2, "Fail : return value Real(%i vs %i)Yours\n", pf, ft); \
	if (pf >= 0 && ft >= 0) {											\
		if (strcmp(ft_dst, pf_dst))										\
			dprintf(2,													\
					"%sFail : strings differ\n|%s| (Real) vs\n|%s| (Yours)\n%s", \
					"\e[1;31m", pf_dst, ft_dst, "\e[0m");				\
		else															\
			dprintf(1, "\e[1;32m|%s|\e[0m\n", ft_dst);					\
	}

int		main(int ac, char **av)
{
	char	*pf_dst;
	char	*ft_dst;
	int		pf;
	int		ft;

	TEST("1-Simple String.", 0);
	TEST("2-Stringception _%s_", "Hello World");
	TEST("%s", "The End");
	(void)ac;
	(void)av;
	return (0);
}