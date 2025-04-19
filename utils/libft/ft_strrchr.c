/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharkho <akharkho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:48:55 by akharkho          #+#    #+#             */
/*   Updated: 2025/04/18 11:00:00 by akharkho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		len;
	char	ch;

	len = (int)ft_strlen(s);
	ch = (char)c;
	if (ch == '\0')
		return ((char *)(s + len));
	while (len >= 0)
	{
		if (s[len] == ch)
			return ((char *)(s + len));
		len--;
	}
	return (NULL);
}
