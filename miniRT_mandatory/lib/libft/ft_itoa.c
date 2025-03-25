/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 17:01:52 by dlemaire          #+#    #+#             */
/*   Updated: 2024/05/16 22:12:44 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> itoa converts an integer to a  string
//-> returns NULL if mem alloc failure
//-> returns the pointer to the string otherwise

#include "libft.h"

static size_t	ft_digitlen(long n)
{
	size_t	len;

	if (n == 0)
		return (1);
	len = 0;
	if (n < 0)
	{
		n *= -1;
		len++;
	}
	while (n > 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	long	n_long;
	size_t	index;
	size_t	str_length;
	char	*str;

	n_long = (long)n;
	str_length = ft_digitlen(n_long);
	str = malloc(sizeof(char) * (str_length + 1));
	if (!str)
		return (NULL);
	index = 0;
	if (n_long == 0)
		str[index] = '0';
	if (n_long < 0)
	{
		str[0] = '-';
		n_long *= -1;
	}
	while (n_long > 0)
	{
		str[(str_length - 1) - index++] = (n_long % 10) + '0';
		n_long /= 10;
	}
	str[str_length] = '\0';
	return (str);
}
