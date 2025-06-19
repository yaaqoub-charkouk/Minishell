#include "utils.h"

void	close_read_fd(t_tree *node)
{
	if (node->red.in_fd != -1)
	{
		close(node->red.in_fd);
		node->red.in_fd = -1;
	}
}

int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while (*str1)
	{
		if (*str1 != *str2)
			return (*str1 - *str2);
		str1++;
		str2++;
	}
	return (*str1 - *str2);
}

int	check_sep(char c, char sep)
{
	return (c == sep);
}

char	**free_strs(char **strs, int i)
{
	while (i >= 0)
		free(strs[i--]);
	free(strs);
	return (NULL);
}
