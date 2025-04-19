/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharkho <akharkho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 21:05:24 by akharkho          #+#    #+#             */
/*   Updated: 2025/04/18 11:00:00 by akharkho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	*ft_memcpy(void *dst, const	void *src, size_t n)
{
	char		*destination;
	const char	*source;

	if (dst == src)
		return (dst);
	destination = (char *)dst;
	source = (const char *)src;
	while (n > 0)
	{
		*destination = *source;
		destination++;
		source++;
		n--;
	}
	return (dst);
}
