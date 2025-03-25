/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 17:45:22 by dlemaire          #+#    #+#             */
/*   Updated: 2024/05/16 18:57:22 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> atoi converts initial portion of string pointed to by nptr to int
//-> does not detect errors
//-> returns 0 if no valid conversion, if more than 1 sign
//-> returns the converted value otherwise

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int	result;
	int	sign_value;
	int	sign_counter;

	result = 0;
	sign_value = 1;
	sign_counter = 0;
	while (*nptr == ' ' || (*nptr >= '\t' && *nptr <= '\r'))
		nptr++;
	while (*nptr == '+' || *nptr == '-')
	{
		if (sign_counter >= 1)
			return (0);
		if (*nptr == '-')
			sign_value = -1;
		sign_counter++;
		nptr++;
	}
	while (ft_isdigit(*nptr))
	{
		result = (result * 10) + (*nptr - '0');
		nptr++;
	}
	return (result * sign_value);
}
