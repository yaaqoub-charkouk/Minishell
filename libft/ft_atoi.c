/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharkho <akharkho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:38:35 by akharkho          #+#    #+#             */
/*   Updated: 2025/06/18 21:39:41 by akharkho         ###   ########.fr       */
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

static	int	check_overflow(long long rs)
{
	return (((rs > LLONG_MAX / 10) || (rs == LLONG_MAX / 10)));
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
		if (s == 1)
		{
			if (check_overflow(rs) && (*str - '0' > (LLONG_MAX % 10)))
				*overflow = 1;
		}
		else if (s == -1)
		{
			if (check_overflow(rs) && ((*str - '0' > -(LLONG_MIN % 10))))
				*overflow = 1;
		}
		if (*overflow)
			break ;
		rs = rs * 10 + (*str - '0');
		str++;
	}
	return (rs * s);
}
