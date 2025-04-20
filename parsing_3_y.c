#include "minishell.h"

t_type_node	is_operator(char *content)
{
	if (ft_strncmp(content, "|", 2) == 0 
        || ft_strncmp(content, ">", 2) == 0 
        || ft_strncmp(content, "<", 2) == 0 
        || ft_strncmp(content, ">>", 3) == 0 
        || ft_strncmp(content, "<<", 3) == 0 
        || ft_strncmp(content, "&&", 3) == 0 
        || ft_strncmp(content, "||", 3) == 0)
        return (1);
	return (0);
}



t_list    *tokenize(char	*line)
{
    int        i;
    int        start;
    t_list    *head;
    t_list    *new_node;
    t_list    *list;
    void      *temp;

    temp = line;
    i = 0;
    while (*line)
    {
        new_node = ft_lstnew
        start = i;
        if (is_operator(line))
        {
            ft_substr(line, start, i - start);
        }
        
        


    }
}