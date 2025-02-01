/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 14:04:36 by dlemaire          #+#    #+#             */
/*   Updated: 2024/05/16 19:57:15 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> bzero erases data in memory by overwriting with zeros ('\0')
//-> it starts at the location pointed to by s and covers n bytes
//-> if n = 0, function does nothing

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*ps;

	ps = (unsigned char *)s;
	while (n--)
	{
		*ps = '\0';
		ps++;
	}
}
