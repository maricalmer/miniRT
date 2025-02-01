/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 11:13:55 by dlemaire          #+#    #+#             */
/*   Updated: 2024/05/16 21:07:20 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> calloc contiguously allocates memory for n elements (nmemb) of size bytes
//-> returns NULL if nmemb * size = int overflow or if mem alloc failure
//-> returns pointer to allocated memory and memory set to 0
//-> returns pointer malloc(0) even if size or nmemb = 0

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*p;
	size_t	max_value;
	size_t	total_size;

	max_value = (size_t) - 1;
	if (size && nmemb > (max_value / size))
		return (NULL);
	total_size = nmemb * size;
	p = malloc(total_size);
	if (!p)
		return (NULL);
	ft_bzero(p, (total_size));
	return (p);
}
