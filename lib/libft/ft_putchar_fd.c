/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 17:07:01 by dlemaire          #+#    #+#             */
/*   Updated: 2024/05/11 17:08:01 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> outputs the character ’c’ to the given file descriptor

#include "libft.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
