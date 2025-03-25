/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 15:02:26 by dlemaire          #+#    #+#             */
/*   Updated: 2024/05/17 14:07:49 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> copies n bytes from src to dest
//-> assumes mem areas do not overlap
//-> returns pointer to dest

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t				index;
	unsigned char		*udest;
	const unsigned char	*usrc;

	udest = (unsigned char *)dest;
	usrc = (const unsigned char *)src;
	if (dest == NULL && src == NULL)
		return (NULL);
	index = 0;
	while (index < n)
	{
		udest[index] = usrc[index];
		index++;
	}
	return (dest);
}
