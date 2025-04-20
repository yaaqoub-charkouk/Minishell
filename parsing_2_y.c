//building tree

#include "minishell.h"

t_list	*tokenize(char	*line);



t_type_node	get_type(char *content)
{
	if (ft_strncmp(content, "|", 2) == 0)
		return (PIPE);
	else if (ft_strncmp(content, ">", 2) == 0)
		return (REDIRECTION_OUT);
	else if (ft_strncmp(content, "<", 2) == 0)
		return (REDIRECTION_IN);
	else if (ft_strncmp(content, ">>", 3) == 0)
		return (APPEND);
	else if (ft_strncmp(content, "<<", 3) == 0)
		return (HEREDOC);
	else if (ft_strncmp(content, "&&", 3) == 0)
		return (AND);
	else if (ft_strncmp(content, "||", 3) == 0)
		return (OR);
	else
		return (CMD);
}

t_list	*tokenize(char	*line)
{
	t_list	*head;
	t_list	*new_node;
	t_list	*list;
	char	**tokens;
	int		i;

	tokens = ft_split(line, ' ');
	if (!tokens)
		return (NULL);
	i = 0;
	while (tokens[i])
	{
		new_node = ft_lstnew(tokens[i]);
		if (!new_node)
			return (free_matrix(tokens), NULL); // node creation failed ;
		new_node->type = get_type(tokens[i]); // get the type the current node ;
		if (i == 0)
		{
			head = new_node;
			list = head;
		}
		else
		{
			list->next = new_node;
			list = new_node;
		}
		i++;
	}
	return (free_matrix(tokens), head);
}






























// t_tree	*create_tree(t_list *list)
// {
// 	 t_tree	*tree_root;
	 
// 	 if (!list)
// 		 return (NULL);
// 	 tree_root = malloc(sizeof(t_tree));
// 	 if (!tree_root)
// 		 return (NULL);
// 	tree_root->cmd = NULL;
// 	tree_root->left_node = NULL;
// 	tree_root->right_node = NULL;
// 	if (list->type == CMD)
// 	{
// 		tree_root->type = CMD;
// 		tree_root->cmd = malloc(sizeof(char *) * 2);
// 		if (!tree_root->cmd)
// 			return (NULL);
// 		tree_root->cmd[0] = ft_strdup(list->content);
// 		tree_root->cmd[1] = NULL;
// 	}
// 	list = list->next;
// 	if (list)
// 	{
// 		if (list->type == PIPE || list->type == AND || list->type == OR)
// 		{
// 			tree_root->type = list->type;
// 			if (list->next)
// 				tree_root->left_node = create_tree(list->next);
// 			if (list->next->next)
// 				tree_root->right_node = create_tree(list->next->next);
// 		}
// 	}
// 	return (tree_root);
// }
