/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sciftci <sciftci@student.42kocaeli.com.tr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 22:30:35 by sciftci           #+#    #+#             */
/*   Updated: 2022/06/29 10:17:53 by sciftci          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_sep(char c, char const *sep)
{
	size_t	i;

	i = 0;
	while (sep[i])
	{
		if (sep[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	len;
	char	*trimmed;

	if (!s1)
		return (NULL);
	i = 0;
	while (s1[i] && is_sep(s1[i], set))
		i++;
	len = ft_strlen(s1);
	while (is_sep(s1[len - 1], set) && len - 1 >= i)
		len--;
	trimmed = malloc(sizeof(char) * (len - i + 1));
	if (!trimmed)
		return (NULL);
	ft_strlcpy(trimmed, s1 + i, len - i + 1);
	return (trimmed);
}
