// define how the tree will be , and build it .

#include "minishell.h"

t_list	*new_node(char *content)
{
	t_list	*new;
	
	new = malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->content = ft_strdup(content);
	if (!new->content)
	{
		free(new);
		return (NULL);
	}
	new->type = get_type(content);
	new->next = NULL;
	return (new);
}

void	free_matrix(char **tokens) //free the matrix returned by ft split
{
	int	i;
	
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
	tokens = NULL;
}