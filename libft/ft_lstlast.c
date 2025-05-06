/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: sciftci <sciftci@student.42kocaeli.com.tr> +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2022/06/28 18:36:07 by sciftci           #+#    #+#             */
/*   Updated: 2022/06/28 18:36:07 by sciftci          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*dup;

	dup = lst;
	while (dup)
	{
		if (dup->next == NULL)
			return (dup);
		dup = dup->next;
	}
	return (NULL);
}
