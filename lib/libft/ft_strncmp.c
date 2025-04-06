/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hruiz-fr <hruiz-fr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 12:44:09 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/26 11:55:08 by hruiz-fr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> compares first n bytes of s1 and s2
//-> comparison is done using unsigned characters
//-> returns diff s1 - s2

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	while (n--)
	{
		if (*s1 != *s2 || *s1 == '\0')
			return ((unsigned char)*s1 - (unsigned char)*s2);
		s1++;
		s2++;
	}
	return (0);
}
