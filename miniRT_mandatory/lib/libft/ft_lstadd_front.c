/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 17:05:45 by dlemaire          #+#    #+#             */
/*   Updated: 2024/05/16 22:59:51 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> ft_lstadd_front adds the node ’new’ at the beginning of the list

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!new)
		return ;
	new->next = *lst;
	*lst = new;
}
