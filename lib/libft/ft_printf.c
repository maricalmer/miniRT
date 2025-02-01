/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 17:05:29 by dlemaire          #+#    #+#             */
/*   Updated: 2025/01/31 02:42:54 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	handle_conversion(char conversion_specifier, va_list args)
{
	int	counter;

	counter = 0;
	if (conversion_specifier == 'c')
		counter += ft_printc(va_arg(args, int));
	else if (conversion_specifier == 's')
		counter += ft_prints(va_arg(args, const char *));
	else if (conversion_specifier == 'd' || conversion_specifier == 'i')
		counter += ft_printd(va_arg(args, int));
	else if (conversion_specifier == 'u')
		counter += ft_printud(va_arg(args, unsigned int));
	else if (conversion_specifier == 'x')
		counter += ft_printx(va_arg(args, unsigned int), "0123456789abcdef");
	else if (conversion_specifier == 'X')
		counter += ft_printx(va_arg(args, unsigned int), "0123456789ABCDEF");
	else if (conversion_specifier == 'p')
		counter += ft_printp(va_arg(args, void *));
	else if (conversion_specifier == '%')
		counter += ft_printc('%');
	return (counter);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		number_of_printed_chars;
	int		index;

	if (!format)
		return (-1);
	va_start(args, format);
	index = 0;
	number_of_printed_chars = 0;
	while (format[index])
	{
		if (format[index] == '%')
			number_of_printed_chars += handle_conversion(format[++index], args);
		else
			number_of_printed_chars += ft_printc(format[index]);
		if (number_of_printed_chars < 0)
			return (-1);
		index++;
	}
	va_end(args);
	return (number_of_printed_chars);
}
