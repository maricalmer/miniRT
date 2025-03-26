/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:41:51 by dlemaire          #+#    #+#             */
/*   Updated: 2024/06/26 14:56:28 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> concatenates s1 and s2 into new str after mem alloc
//-> returns NULL if mem alloc failure, new str otherwise

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*concat_string;
	size_t	s1_length;
	size_t	s2_length;

	if (!s1 || !s2)
		return (NULL);
	s1_length = ft_strlen(s1);
	s2_length = ft_strlen(s2);
	concat_string = malloc(sizeof(char) * (s1_length + s2_length + 1));
	if (concat_string == NULL)
		return (NULL);
	ft_strlcpy(concat_string, s1, s1_length + 1);
	ft_strlcpy(concat_string + (s1_length), s2, s2_length + 1);
	return (concat_string);
}
