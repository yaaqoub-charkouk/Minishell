#include "../minishell.h"

int	ft_wild_size(t_wild *lst)
{
	int	c;

	c = 0;
	while (lst)
	{
		lst = lst->next;
		c++;
	}
	return (c);
}

char	**wild_struct_to_char(t_wild  *wild)
{
	int		size;
	int		i;
	char	**wild_char;

	i = 0;
	size = ft_wild_size(wild);
	wild_char = malloc((size + 1) * sizeof(char *));
  	if(!wild_char)
    	return (NULL);
	while (i < size)
	{
		wild_char[i] = ft_strdup(wild->file);
		wild = wild->next;
		i++;
	}
	wild_char[i] = NULL;
	return (wild_char);
}

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
void	ft_add_back_wild(t_wild **lst, t_wild *new)
{
	t_wild	*last;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last = *lst;
	while (last->next)
		last = last->next;
	last->next = new;
}

static int	matches_pattern(char *pattern, const char *filename)
{
	if(!*pattern && !*filename)//if both are finished it means they match
		return (1);
	if (!*pattern && *filename)//if pattern sala wlakin filename baki no match
		return (0);
	if(*pattern == '*')
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
	t_wild			*wild;

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
			continue;
		}
		if (matches_pattern(pattern, entry->d_name))
			ft_add_back_wild(&wild, ft_new_wild(entry->d_name));
		
		entry = readdir(dir);
	}
	closedir(dir);
	return (wild_struct_to_char(wild));
}
