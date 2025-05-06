/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sciftci <sciftci@student.42kocaeli.com.tr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 18:45:17 by sciftci           #+#    #+#             */
/*   Updated: 2022/06/28 18:45:17 by sciftci          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	t_list	*dup;

	if (f == NULL)
		return ;
	dup = lst;
	while (dup != NULL)
	{
		(*f)(dup->content);
		dup = dup->next;
	}
	return ;
}
