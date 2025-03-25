/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 17:07:01 by dlemaire          #+#    #+#             */
/*   Updated: 2024/05/11 17:08:01 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> outputs the string ’s’ to the given file descriptor

#include "libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	size_t	s_len;

	if (!s)
		return ;
	s_len = ft_strlen(s);
	write(fd, s, s_len);
}
