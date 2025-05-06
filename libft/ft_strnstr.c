/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sciftci <sciftci@student.42kocaeli.com.tr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/21 14:22:21 by sciftci           #+#    #+#             */
/*   Updated: 2022/06/28 18:38:33 by sciftci          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	needle_len;
	size_t	cursor;

	needle_len = ft_strlen(needle);
	if (needle_len == 0)
		return ((char *)haystack);
	if (len == 0)
		return ((char *) NULL);
	cursor = 0;
	while (cursor <= (len - needle_len) && haystack[cursor] != '\0')
	{
		if (ft_strncmp(&haystack[cursor], needle, needle_len) == 0)
			return ((char *)&haystack[cursor]);
		cursor++;
	}
	return ((char *) NULL);
}
