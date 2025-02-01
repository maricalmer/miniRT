/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 17:07:01 by dlemaire          #+#    #+#             */
/*   Updated: 2024/05/11 17:08:01 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> applies f(index, char) to each character of s
//-> new string is created to save outputs of successive f() calls
//-> mem for new stringg is dynamically allocated

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*result;
	size_t	s_length;
	size_t	index;

	if (!s || !f)
		return (NULL);
	s_length = ft_strlen(s);
	result = malloc(sizeof(char) * (s_length + 1));
	if (!result)
		return (NULL);
	index = 0;
	while (s[index])
	{
		result[index] = f(index, s[index]);
		index++;
	}
	result[index] = '\0';
	return (result);
}
