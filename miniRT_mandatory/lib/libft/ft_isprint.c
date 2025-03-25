/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 17:58:14 by dlemaire          #+#    #+#             */
/*   Updated: 2024/05/16 21:31:00 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> checks for any printable character
//-> returns 1 if true and 0 if false

#include "libft.h"

int	ft_isprint(int c)
{
	return (c >= ' ' && c <= '~');
}
