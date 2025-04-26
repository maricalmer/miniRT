/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 18:23:46 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/26 18:39:31 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> locates the of the null char
//-> substracts from it the position of the 1st char

#include "libft.h"

size_t	ft_strlen(const char *s)
{
	const char	*p;

	p = s;
	while (*s)
		s++;
	return (s - p);
}
