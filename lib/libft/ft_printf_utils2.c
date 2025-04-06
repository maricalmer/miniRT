/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hruiz-fr <hruiz-fr@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 02:53:10 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/26 11:53:24 by hruiz-fr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printd(int nbr)
{
	int	counter;

	counter = 0;
	if (nbr == -2147483648)
	{
		counter += ft_prints("-2147483648");
		return (counter);
	}
	if (nbr < 0)
	{
		counter += ft_printc('-');
		nbr = -nbr;
	}
	counter += ft_printud((unsigned int)nbr);
	return (counter);
}

int	ft_printc(const char c)
{
	int				bytes_written;
	unsigned char	uc;

	uc = (unsigned char) c;
	bytes_written = write(STDOUT_FILENO, &uc, 1);
	if (bytes_written == -1)
		return (-1);
	else
		return (1);
}
