/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharkho <akharkho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:38:35 by akharkho          #+#    #+#             */
/*   Updated: 2025/06/17 17:55:13 by akharkho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	space_sign(const char **s, int *sign)
{
	while (**s == 32 || (**s >= 9 && **s <= 13))
		(*s)++;
	if (**s == '+' || **s == '-')
	{
		if (**s == '-')
			*sign *= -1; 
		(*s)++;
	}
}

long long	ft_atoi(const char *str, int *overflow)
{
	long long	rs;
	int			s;

	s = 1;
	*overflow = 0;
	space_sign(&str, &s);
	rs = 0;
	while (ft_isdigit(*str))
	{
		if (rs > (LONG_MAX - *str - '\0') / 10)
		{
			*overflow = 1;
			break;
		}
		rs = rs * 10 + (*str - '0');
		str++;
	}
	return (rs * s);
}
