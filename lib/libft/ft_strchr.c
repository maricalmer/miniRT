/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 22:41:34 by dlemaire          #+#    #+#             */
/*   Updated: 2024/06/26 14:57:44 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> locates 1st occurence of c (converted as a char) in str pointed to by s
//-> it should be able to locate '\0' too
//-> returns pointer to first occurence

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	*ps;

	ps = (char *)s;
	while (*ps)
	{
		if (*ps == (char)c)
			return (ps);
		ps++;
	}
	if ((char)c == '\0')
		return (ps);
	return (NULL);
}
