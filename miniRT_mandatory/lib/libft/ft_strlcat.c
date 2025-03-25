/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 20:36:48 by dlemaire          #+#    #+#             */
/*   Updated: 2024/05/20 13:26:09 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> appends src to dst up to (size - dst_len - 1)
//-> size must include byte for NULL terminator
//-> dst, src and result should be null-terminated
//-> returns length of dst + length of src

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	index;
	size_t	dst_len;
	size_t	src_len;

	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	if (size == 0 || size <= dst_len)
		return (src_len + size);
	index = 0;
	while (src[index] && index < size - dst_len - 1)
	{
		dst[dst_len + index] = src[index];
		index++;
	}
	dst[dst_len + index] = '\0';
	return (dst_len + src_len);
}
