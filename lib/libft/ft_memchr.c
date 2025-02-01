/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 15:16:48 by dlemaire          #+#    #+#             */
/*   Updated: 2024/05/17 13:24:57 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> locates the first occurrence of a specific byte value in a memory block
//-> both c and byte of mem pointed to by s are interpreted as unsigned chars
//-> returns pointer or NULL if nothing found

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*ps;
	unsigned char	uc;

	ps = (unsigned char *) s;
	uc = (unsigned char) c;
	while (n--)
	{
		if (*ps == uc)
			return (ps);
		ps++;
	}
	return (NULL);
}
