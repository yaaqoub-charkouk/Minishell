#include "../minishell.h"

t_wild	*ft_new_wild(void *content)
{
	t_wild	*node;

	node = (t_wild *)malloc(sizeof(t_wild));
	if (!node)
		return (NULL);
	node->file = ft_strdup(content);
	node->next = NULL;
	return (node);
}
int	ft_rev_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*str1;
	unsigned char	*str2;
	size_t			i;

	if (n == 0)
		return (0);
	i = 0;
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while (i < n - 1 && (str1[i] != '\0'))
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
		i++;
	}
	return (str1[i] - str2[i]);
}
static int	matches_pattern(char *pattern, const char *filename, t_wild *wild)
{
	char	*pos;
	int		len;
	int		flag;

	flag = 0;
	pos = ft_strchr(pattern, '*');
	if (pos)
	{
		len = ft_strlen(pattern);
		if (pattern[0] == '*')
		{
			flag = 1;
			pattern++;
			len -= 1;
		}
		else if (pattern[len - 1] == '*')
		{
			flag = 3;
			pattern[len - 1] = '\0';
			len -= 1;
		}
		else
			flag = 2;
	}
	if (flag == 1 && ft_strncmp(pattern, filename, len) == 0)
		ft_new_wild(filename);
	if (flag == 3)
	{

	}		

}

int	expand_wildcard(char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*str;
	t_wild			*wild;

	dir = opendir(".");
	if (!dir)
	{
		perror("minishell: opendir");
		return (EXIT_FAILURE);
	}
	str = NULL;
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] == '.')
		{
			entry = readdir(dir);
			continue;
		}
		
		if (matches_pattern(pattern, entry->d_name, wild))
		{

		}
		
		entry = readdir(dir);
	}
	close(dir);
}

int main(void)
{
	expand_wildcard("*");
	return (0);
}