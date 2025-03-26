/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_iswhitespace.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 17:41:46 by dlemaire          #+#    #+#             */
/*   Updated: 2025/02/23 12:42:20 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> checks if character is part of the white space chars
//-> returns 1 if true and 0 if false

#include "libft.h"

int	ft_iswhitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}
