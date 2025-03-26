/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 17:07:01 by dlemaire          #+#    #+#             */
/*   Updated: 2024/05/11 17:08:01 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> applies f() at each char of s
//-> f takes index and address of char

#include "libft.h"

void ft_striteri(char *s, void (*f)(unsigned int, char *))
{
  size_t index;

  if (!s || !f)
    return;
  index = 0;
  while (s[index])
  {
    (*f)(index, &s[index]);
    index++;
  }
}
