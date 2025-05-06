/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sciftci <sciftci@student.42kocaeli.com.tr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 16:28:46 by sciftci           #+#    #+#             */
/*   Updated: 2022/06/28 18:26:24 by sciftci          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	cursor;

	if (n == 0)
		return (0);
	cursor = 0;
	while (cursor < (n - 1) && s1[cursor] && s2[cursor])
	{
		if (s1[cursor] != s2[cursor])
			break ;
		cursor++;
	}
	return (((unsigned char *)s1)[cursor] - ((unsigned char *)s2)[cursor]);
}
