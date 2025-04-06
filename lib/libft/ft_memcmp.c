/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 15:51:53 by dlemaire          #+#    #+#             */
/*   Updated: 2024/05/17 13:34:08 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> compares the first n bytes of memory areas s1 and s1
//-> both s1 and s2 are interpreted as unsigned chars
//-> returns 0 if n = 0
//-> returns difference between first n bytes of s1 and s2

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*ps1;
	unsigned char	*ps2;

	ps1 = (unsigned char *)s1;
	ps2 = (unsigned char *)s2;
	while (n--)
	{
		if (*ps1 != *ps2)
			return (*ps1 - *ps2);
		ps1++;
		ps2++;
	}
	return (0);
}
