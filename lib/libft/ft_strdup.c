/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 12:51:57 by dlemaire          #+#    #+#             */
/*   Updated: 2024/06/27 17:31:27 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> allocates mem for copy of str to then operate copy
//-> returns pointer to str or NULL if mem alloc failure

#include "libft.h"

char	*ft_strdup(const char *s)
{
	int		index;
	int		s_len;
	char	*copy;

	s_len = ft_strlen(s);
	copy = malloc((s_len + 1) * sizeof(char));
	if (!copy)
		return (NULL);
	index = 0;
	while (index < s_len)
	{
		copy[index] = s[index];
		index++;
	}
	copy[index] = '\0';
	return (copy);
}
