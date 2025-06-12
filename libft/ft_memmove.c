/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycharkou <ycharkou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 10:05:11 by akharkho          #+#    #+#             */
/*   Updated: 2025/06/12 17:42:30 by ycharkou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char		*destanation;
	const char	*source;

	if (dst == src)
		return (dst);
	destanation = (char *)dst;
	source = (const char *)src;
	if (!dst && !src)
		return (dst);
	if (dst > src)
	{
		destanation += len;
		source += len;
		while (len-- > 0)
			*(--destanation) = *(--source);
	}
	else
	{
		while (len-- > 0)
			*destanation++ = *source++;
	}
	return (dst);
}
