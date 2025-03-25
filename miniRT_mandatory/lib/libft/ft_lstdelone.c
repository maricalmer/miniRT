/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 17:08:21 by dlemaire          #+#    #+#             */
/*   Updated: 2024/05/17 12:14:29 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//-> frees node content's memory with del() and node with free()
//-> memory of ’next’ must not be freed

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	if (lst && del)
	{
		del(lst->content);
		free(lst);
	}
}
