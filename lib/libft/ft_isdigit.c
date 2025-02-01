/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 16:09:19 by dlemaire          #+#    #+#             */
/*   Updated: 2024/05/16 21:26:47 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> checks if character is a digit
//-> returns 1 if true and 0 if false

#include "libft.h"

int	ft_isdigit(int c)
{
	return ((c >= '0' && c <= '9'));
}
