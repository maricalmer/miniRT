/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 12:53:49 by dlemaire          #+#    #+#             */
/*   Updated: 2024/05/07 12:54:30 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> returns copy of ’s1’ with characters specified in ’set’ removed 
//   from beginning and end of string

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	char	*str;

	str = 0;
	if (s1 && set)
	{
		start = 0;
		end = ft_strlen(s1);
		while (s1[start] && ft_strchr(set, s1[start]))
			start++;
		while (s1[end - 1] && ft_strchr(set, s1[end - 1]) && end > start)
			end--;
		str = malloc(sizeof(char) * (end - start + 1));
		if (!str)
			return (NULL);
		ft_strlcpy(str, s1 + start, (end - start + 1));
	}
	return (str);
}
