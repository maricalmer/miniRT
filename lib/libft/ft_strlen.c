/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 18:23:46 by dlemaire          #+#    #+#             */
/*   Updated: 2024/04/24 18:57:09 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> calculates the length of the string pointed to by s
//-> excludes the terminating null byte

#include "libft.h"

size_t	ft_strlen(const char *s)
{
	size_t	length;

	length = 0;
	while (s[length])
	{
		length++;
	}
	return (length);
}
