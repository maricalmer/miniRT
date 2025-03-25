/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 13:58:25 by dlemaire          #+#    #+#             */
/*   Updated: 2024/05/02 12:43:53 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> returns a substring of string ’s’
//-> substring begins at index ’start’
//-> max size of ’len'

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	s_len;
	size_t	buffer_size;
	char	*substring;

	s_len = ft_strlen(s);
	if (start > s_len)
	{
		start = 0;
		len = 0;
	}
	else if (len + start > s_len)
		len = s_len - start;
	buffer_size = len + 1;
	substring = malloc(buffer_size * sizeof(char));
	if (!substring)
		return (NULL);
	ft_strlcpy(substring, s + start, buffer_size);
	return (substring);
}
