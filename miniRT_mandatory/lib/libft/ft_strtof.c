/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtof.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricalmer <maricalmer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 17:45:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/02/26 23:13:04 by maricalmer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> strtof converts portion of string pointed to by str to float
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

static float	ft_parse_integer(const char **str)
{
	double	result;

	result = 0.0;
	while (ft_isdigit(**str))
	{
		result = result * 10.0 + (**str - '0');
		(*str)++;
	}
	return (result);
}

static float	ft_parse_fraction(const char **str)
{
	double	fraction;
	double	divisor;

	fraction = 0.0;
	divisor = 10.0;
	if (**str == '.')
	{
		(*str)++;
		while (ft_isdigit(**str))
		{
			fraction += (**str - '0') / divisor;
			divisor *= 10.0;
			(*str)++;
		}
	}
	return (fraction);
}

static float   ft_handle_limits_and_endptr(double result, const char *str, const char *start, char **endptr)
{
    if (result > FLT_MAX)
    {
        errno = ERANGE;
        return HUGE_VALF;
    }
    if (result < -FLT_MAX)
    {
        errno = ERANGE;
        return -HUGE_VALF;
    }
    if (result != 0.0 && ft_fabs(result) < FLT_MIN)
    {
        errno = ERANGE;
        return 0.0f;
    }
    if (endptr)
    {
        if (*str)
            *endptr = (char *)str;
        else
            *endptr = (char *)start;
    }
    return (float)result;
}

float	ft_strtof(const char *str, char **endptr)
{
	double	result;
	int		sign;
    const char *start;

    start = str;
    while (ft_iswhitespace(*str))
		str++;
	sign = ft_parse_sign(&str);
	result = ft_parse_integer(&str) + ft_parse_fraction(&str);
	result *= sign;
    return (ft_handle_limits_and_endptr(result, str, start, endptr));
}

