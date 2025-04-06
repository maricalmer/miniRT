/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 16:32:14 by dlemaire          #+#    #+#             */
/*   Updated: 2024/04/29 17:41:41 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> locates 1st occurrence of string little in string big
//-> not more than len characters are searched

#include "libft.h"

char	*ft_strnstr(const char *big, const char *small, size_t len)
{
	size_t	index_big;
	size_t	index_small;

	index_big = 0;
	if (*small == '\0')
		return ((char *)big);
	while (index_big < len && big[index_big] != '\0')
	{
		index_small = 0;
		while (small[index_small] == big[index_big + index_small]
			&& small[index_small] != '\0'
			&& (index_big + index_small) < len)
		{
			index_small++;
		}
		if (small[index_small] == '\0')
			return ((char *)&big[index_big]);
		index_big++;
	}
	return (NULL);
}
