#include "utils.h"

t_list	*add_cmd_options(t_list **args_list, char **args, int i)
{
	if (i == 0)
		*args_list = NULL;
	if (!args || !*args)
		return (printf("add cmd options NULL \n"), NULL); // delete printf;
	while (args[i])
	{
		ft_lstadd_back(args_list, ft_lstnew(ft_strdup(args[i]))); // because we fre args at expand ; 
		i++;
	}
	return (*args_list);
}

char	**list_to_char(t_list  *args_list)
{
	int		size;
	int		i;
	char	**args_char;

	if (!args_list)
		return (printf("list to char NULL \n"), NULL);
	i = 0;
	size = ft_lstsize(args_list);
	args_char = malloc((size + 1) * sizeof(char *));
	if(!args_char)
     return (NULL);
	while (i < size)
	{
		args_char[i] = ft_strdup(args_list->content); // no need to allocate;
		args_list = args_list->next;
		i++;
	}
	args_char[i] = NULL;
	return (args_char); // need to be freed ;
}

int	is_redirection(t_type_node type)
{
	return (type == REDIRECTION_IN || type == REDIRECTION_OUT 
			|| type == APPEND || type == HEREDOC);
}

int	is_operator(t_type_node type)
{
	return (type == PIPE || type == OR || type == AND);
}

int	get_operator_len(t_type_node type)
{
	if (type == APPEND || type == HEREDOC || type == AND || type == OR)
		return (2);
	return (1);
}