/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 10:57:47 by dlemaire          #+#    #+#             */
/*   Updated: 2024/04/29 12:08:13 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> returns pointer to the last occurrence of the character c in the string s
//-> returns NULL if the character is not found
//-> if c is specified as '\0', returns a pointer to terminator

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		len_s;
	char	*ps;

	len_s = ft_strlen(s);
	ps = (char *)s + len_s;
	if ((char)c == '\0')
		return (ps);
	while (len_s-- >= 0)
	{
		if (*ps == (char)c)
			return (ps);
		ps--;
	}
	return (NULL);
}
