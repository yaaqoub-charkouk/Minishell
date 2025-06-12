#include "../minishell.h"

static int	matches_pattern(char *pattern, const char *filename)
{
	if (!*pattern && !*filename)//if both are finished it means they match
		return (1);
	if (!*pattern && *filename)//if pattern sala wlakin filename baki no match
		return (0);
	if (*pattern == '*')
	{
		if (matches_pattern(pattern + 1, filename))//skip the * to check the rest of the pattern
			return (1);
		if (*filename && matches_pattern(pattern, filename + 1))//check if the rest of the filename matches the pattern
			return (1);
		return (0);
	}
	else if (*pattern == *filename)
		return (matches_pattern(pattern + 1, filename + 1));
	return (0);
}

char	**expand_wildcard(char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	t_list			*wild;

	wild = NULL;
	dir = opendir(".");
	if (!dir)
		return (perror("minishell: opendir"), NULL);
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] == '.' && pattern[0] != '.')
		{
			entry = readdir(dir);
			continue ;
		}
		if (matches_pattern(pattern, entry->d_name))
			ft_lstadd_back(&wild, ft_lstnew(entry->d_name));
		entry = readdir(dir);
	}
	closedir(dir);
	return (list_to_char(wild));
}
