/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fabs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 17:45:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/26 19:17:23 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> returns the absolute value of the floating-point number x

#include "libft.h"

float	ft_fabs(float num)
{
	if (num < 0)
		return (-num);
	else
		return (num);
}
