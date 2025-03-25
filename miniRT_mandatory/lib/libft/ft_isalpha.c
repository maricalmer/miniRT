/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:57:16 by dlemaire          #+#    #+#             */
/*   Updated: 2024/05/16 21:19:40 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> checks for alphabetic characters
//-> returns 1 if true and 0 if false

#include "libft.h"

int	ft_isalpha(int c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}
