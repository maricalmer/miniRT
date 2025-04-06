/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 16:12:23 by dlemaire          #+#    #+#             */
/*   Updated: 2024/04/25 19:35:52 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> copies n bytes from src to dest
//-> should handle overlap and preserve data
//-> returns pointer to dest

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	const unsigned char	*psrc;
	unsigned char		*pdest;

	if (!src && !dest)
		return (NULL);
	psrc = src;
	pdest = dest;
	if (psrc < pdest && psrc + n > pdest)
		while (n--)
			pdest[n] = psrc[n];
	else
		while (n--)
			*pdest++ = *psrc++;
	return (dest);
}
