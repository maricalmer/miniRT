/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 16:29:10 by dlemaire          #+#    #+#             */
/*   Updated: 2024/05/16 21:13:17 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> isalnum checks for alphanumeric characters
//-> returns 1 if true and 0 if false

#include "libft.h"

int	ft_isalnum(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c));
}
