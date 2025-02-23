/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtof.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 17:45:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/02/23 16:47:22 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> strtof converts portion of string pointed to by str to float
//-> does not detect scientific notation
//-> returns 0 if no valid conversion
//-> returns the converted value otherwise and updates the endptr

#include "libft.h"

const char	*ft_skip_whitespaces(const char *str)
{
	while (ft_iswhitespace(*str))
		str++;
	return (str);
}

int	ft_parse_sign(const char **str)
{
	int	sign;

	sign = 1;
	if (**str == '-' || **str == '+')
	{
		if (**str == '-')
			sign = -1;
		(*str)++;
	}
	return (sign);
}

float	ft_parse_integer(const char **str)
{
	float	result;

	result = 0.0f;
	while (ft_isdigit(**str))
	{
		result = result * 10.0f + (**str - '0');
		(*str)++;
	}
	return (result);
}

float	ft_parse_fraction(const char **str)
{
	float	fraction;
	float	divisor;

	fraction = 0.0f;
	divisor = 10.0f;
	if (**str == '.')
	{
		(*str)++;
		while (ft_isdigit(**str))
		{
			fraction += (**str - '0') / divisor;
			divisor *= 10.0f;
			(*str)++;
		}
	}
	return (fraction);
}

float	ft_strtof(const char *str, char **endptr)
{
	float	result;
	int		sign;

	str = ft_skip_whitespaces(str);
	sign = ft_parse_sign(&str);
	result = ft_parse_integer(&str) + ft_parse_fraction(&str);
	if (endptr)
		*endptr = (char *)str;
	return (result * sign);
}
