/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 13:18:20 by dlemaire          #+#    #+#             */
/*   Updated: 2024/05/07 15:04:46 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> splits ’s’ into substrings using ’c’ as a delimiter
//-> allocates mem and stores substrings in array
//-> array must end with NULL pointer
//-> returns array of strings

#include "libft.h"

static int ft_substring_len(char const *str, char delimiter)
{
	int len;

	len = 0;
	while (str[len] && str[len] != delimiter)
		len++;
	return (len);
}

static int ft_substring_amount(char const *s, char delimiter)
{
	size_t index;
	int count;

	index = 0;
	count = 0;
	while (s[index])
	{
		if (s[index] != delimiter)
		{
			count++;
			while (s[index] != delimiter && s[index])
				index++;
		}
		else
			index++;
	}
	return (count);
}

static char **ft_free_array(char **strs, int i)
{
	while (i > 0)
	{
		free(strs[--i]);
	}
	free(strs);
	return (NULL);
}

char **ft_split(char const *s, char delimiter)
{
	int index;
	int substring_length;
	char **strings;

	if (!s)
		return (NULL);
	strings = malloc(sizeof(char *) * (ft_substring_amount(s, delimiter) + 1));
	if (!strings)
		return (ft_free_array(strings, 0));
	index = 0;
	while (*s)
	{
		while (*s == delimiter && *s)
			s++;
		substring_length = ft_substring_len(s, delimiter);
		if (substring_length)
		{
			strings[index++] = ft_substr(s, 0, substring_length);
			if (!strings[index - 1])
				return (ft_free_array(strings, index - 1));
		}
		s += substring_length;
	}
	strings[index] = NULL;
	return (strings);
}
