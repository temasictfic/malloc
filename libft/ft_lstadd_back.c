/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sciftci <sciftci@student.42kocaeli.com.tr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 21:05:01 by sciftci           #+#    #+#             */
/*   Updated: 2022/06/25 21:05:01 by sciftci          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*dup;

	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	dup = *lst;
	while (dup->next)
		dup = dup->next;
	dup->next = new;
}
