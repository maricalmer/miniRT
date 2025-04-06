/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtoi.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hruiz-fr <hruiz-fr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 17:45:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/26 11:59:15 by hruiz-fr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> strtoi converts portion of string pointed to by str to int
//-> does not detect scientific notation
//-> returns 0 if no valid conversion
//-> returns the converted value otherwise and updates the endptr

#include "libft.h"

static int	ft_parse_sign(const char **str)
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

static long long	ft_parse_integer(const char **str)
{
	long long	result;

	result = 0;
	while (ft_isdigit(**str))
	{
		result = result * 10 + (**str - '0');
		(*str)++;
	}
	return (result);
}

static float	ft_handle_limits_and_endptr(double result, const char *str,
	const char *start, char **endptr)
{
	if (result > FLT_MAX)
	{
		errno = ERANGE;
		return (HUGE_VALF);
	}
	if (result < -FLT_MAX)
	{
		errno = ERANGE;
		return (-HUGE_VALF);
	}
	if (result != 0.0 && ft_fabs(result) < FLT_MIN)
	{
		errno = ERANGE;
		return (0.0f);
	}
	if (endptr)
	{
		if (*str)
			*endptr = (char *)str;
		else
			*endptr = (char *)start;
	}
	return ((float)result);
}

int	ft_strtoi(const char *str, char **endptr)
{
	long long	result;
	int			sign;
	const char	*start;

	start = str;
	while (ft_iswhitespace(*str))
		str++;
	sign = ft_parse_sign(&str);
	result = ft_parse_integer(&str) * sign;
	return (ft_handle_limits_and_endptr(result, str, start, endptr));
}
