/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 02:50:12 by dlemaire          #+#    #+#             */
/*   Updated: 2025/01/31 02:53:00 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printx(unsigned int nbr, char *chars)
{
	int	counter;

	counter = 0;
	if (nbr >= 16)
	{
		counter += ft_printx(nbr / 16, chars);
		counter += ft_printx(nbr % 16, chars);
	}
	else
	{
		if (ft_printc(chars[nbr]) == -1)
			return (-1);
		counter++;
	}
	return (counter);
}


int	ft_printud(unsigned int nbr)
{
	int	counter;

	counter = 0;
	if (nbr >= 10)
	{
		counter += ft_printd(nbr / 10);
		counter += ft_printd(nbr % 10);
	}
	else
	{
		if (ft_printc(nbr + '0') == -1)
			return (-1);
		counter++;
	}
	return (counter);
}

int	ft_prints(const char *s)
{
	int		counter;
	char	*null_output;

	counter = 0;
	if (!s)
	{
		null_output = "(null)";
		while (null_output[counter])
		{
			if (ft_printc(null_output[counter]) == -1)
				return (-1);
			counter++;
		}
	}
	else
	{
		while (s[counter])
		{
			if (ft_printc(s[counter]) == -1)
				return (-1);
			counter++;
		}
	}
	return (counter);
}

int	ft_putp(uintptr_t nbr)
{
	char	*hexadecimal_digits;
	int		counter;

	hexadecimal_digits = "0123456789abcdef";
	counter = 0;
	if (nbr >= 16)
	{
		counter += ft_putp(nbr / 16);
		counter += ft_putp(nbr % 16);
	}
	else
	{
		if (ft_printc(hexadecimal_digits[nbr]) == -1)
			return (-1);
		counter++;
	}
	return (counter);
}

int	ft_printp(void *adr)
{
	int			counter;
	uintptr_t	uintadr;
	char		*null_output;

	counter = 0;
	uintadr = (uintptr_t) adr;
	if (!uintadr)
	{
		null_output = "(nil)";
		counter += ft_prints(null_output);
		return (counter);
	}
	counter += ft_prints("0x");
	if (counter < 0)
		return (-1);
	counter += ft_putp(uintadr);
	if (counter < 0)
		return (-1);
	return (counter);
}
