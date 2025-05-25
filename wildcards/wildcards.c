#include "../minishell.h"

static int	matches_pattern(const char *pattern, const char *filename)
{
	const char	*star;

	if (!pattern || !filename)
		return (0);
	star = ft_strchr(pattern, '*');
	if (!star)
		return (ft_strcmp(pattern, filename) == 0);
	if (star == pattern && !pattern[1])
		return (1);
	if (star == pattern)
		return (ft_strnstr(filename, pattern + 1, ft_strlen(filename)) != NULL);
	if (!pattern[1])
		return (ft_strnstr(filename, pattern, ft_strlen(filename)) != NULL);
	return (0);
}

int	expand_wildcard(char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*str;

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
		
		if (matches_pattern(pattern, entry->d_name))
		{

		}
		
		entry = readdir(dir);
	}
	close(dir);
}

int		wildcards(char **args)
{
	
}

int main(void)
{
	expand_wildcard("*");
	return (0);
}