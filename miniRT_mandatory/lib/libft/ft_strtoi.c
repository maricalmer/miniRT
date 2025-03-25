/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtoi.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricalmer <maricalmer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 17:45:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/02/27 00:04:27 by maricalmer       ###   ########.fr       */
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


static int   ft_handle_limits_and_endptr(long long result, const char *str, const char *start, char **endptr)
{
    if (result > INT_MAX)
    {
        errno = ERANGE;
        return (INT_MAX);
    }
    if (result < INT_MIN)
    {
        errno = ERANGE;
        return (INT_MIN);
    }
    if (endptr)
    {
        if (*str)
            *endptr = (char *)str;
        else
            *endptr = (char *)start;
    }
    return (int)result;
}

int	ft_strtoi(const char *str, char **endptr)
{
	long long   result;
	int		    sign;
    const char  *start;

    start = str;
    while (ft_iswhitespace(*str))
		str++;
	sign = ft_parse_sign(&str);
	result = ft_parse_integer(&str) * sign;
    return (ft_handle_limits_and_endptr(result, str, start, endptr));
}
