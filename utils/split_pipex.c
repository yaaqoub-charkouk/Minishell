/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_pipex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharkho <akharkho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 21:50:23 by akharkho          #+#    #+#             */
/*   Updated: 2025/06/20 21:50:24 by akharkho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

static void	update_quotes(char c, int *d, int *s)
{
	if (c == '\"' && !(*s))
		*d = !(*d);
	else if (c == '\'' && !(*d))
		*s = !(*s);
}

static int	word_len(const char *str, char sep)
{
	int	len;
	int	in_d;
	int	in_s;

	len = 0;
	in_d = 0;
	in_s = 0;
	while (str[len] && (!check_sep(str[len], sep) || in_d || in_s))
	{
		update_quotes(str[len], &in_d, &in_s);
		len++;
	}
	return (len);
}

int	count_words(const char *s, char sep)
{
	int	count;
	int	in_d;
	int	in_s;

	count = 0;
	in_d = 0;
	in_s = 0;
	while (*s)
	{
		while (*s && check_sep(*s, sep) && !in_d && !in_s)
			s++;
		count += (*s != 0);
		while (*s && (!check_sep(*s, sep) || in_d || in_s))
			update_quotes(*s++, &in_d, &in_s);
	}
	return (count);
}

static char	**fill_strs(const char *s, char c, char **strs)
{
	int	i;
	int	len;
	int	in_d;
	int	in_s;

	i = 0;
	in_d = 0;
	in_s = 0;
	while (*s)
	{
		while (check_sep(*s, c) && !in_d && !in_s)
			s++;
		if (!*s)
			break ;
		len = word_len(s, c);
		strs[i] = ft_substr(s, 0, len);
		if (!strs[i++])
			return (free_strs(strs, i - 1));
		s += len;
	}
	strs[i] = NULL;
	return (strs);
}

char	**ft_split_pipex(const char *s, char c)
{
	char	**strs;
	int		word_count;

	if (!s)
		return (NULL);
	word_count = count_words(s, c);
	strs = malloc(sizeof(char *) * (word_count + 1));
	if (!strs)
		return (NULL);
	return (fill_strs(s, c, strs));
}
