/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 19:03:47 by dlemaire          #+#    #+#             */
/*   Updated: 2024/04/25 12:56:03 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> fills first n bytes of the mem area pointed to by s with byte c
//-> returns pointer to s

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	value;
	unsigned char	*ps;

	ps = (unsigned char *)s;
	value = (unsigned char)c;
	while (n--)
	{
		*ps = value;
		ps++;
	}
	return (s);
}
