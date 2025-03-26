/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 17:41:46 by dlemaire          #+#    #+#             */
/*   Updated: 2024/05/16 21:27:13 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> checks if character is part of the ASCII set
//-> returns 1 if true and 0 if false

#include "libft.h"

int	ft_isascii(int c)
{
	return (c >= 0 && c <= 127);
}
