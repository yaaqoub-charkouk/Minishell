/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycharkou <ycharkou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 18:16:30 by akharkho          #+#    #+#             */
/*   Updated: 2025/04/26 17:47:03 by ycharkou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

static int	trim(char c, char const *set)
{
	while (*set != '\0')
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set1, char const *set2)
{
	char	*str;
	char	*start;
	char	*final;
	size_t	len;

	if (!s1 || !set1 || !set2)
		return (NULL);
	start = (char *)s1;
	final = (char *)s1 + ft_strlen(s1) - 1;
	while ((trim(*start, set1) || trim(*start, set2)) && start <= final)
		start++;
	while ((trim(*final, set1) || trim(*final, set2)) && final >= start)
		final--;
	len = final - start + 1;
	str = malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	ft_strlcpy(str, start, len + 1);
	return (str);
}
