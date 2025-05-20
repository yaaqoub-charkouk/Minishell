#include "parsing.h"

t_tree	*build_tree_from_rqueue(t_list **current)
{
	t_tree	*node;

	if (!current || !*current)
		return (NULL);

	node = new_tree_node(*current);
	*current = (*current)->next;

	if (node->type != CMD)
	{
		node->right = build_tree_from_rqueue(current); // build the right first , so that the left will be the right->next ,wich will be updated when calling the function 
		node->left = build_tree_from_rqueue(current);
	}
	return (node);
}

void	fill_cmd(t_list *lst)
{
	t_list	*node;
	// t_list	*temp;
	int		redirections;
	int		commands;
	int		is_start;

	is_start = 1;
	redirections = 0;
	commands = 0;
	node = NULL;
	while (lst)
	{
		// printf("node : %s\n", lst->content);

		// if (is_redirection(lst->type) && (!lst->next ||lst->next->type == CMD) 
		// 	&& (!lst->next->next || lst->next->next->type != CMD))
		// {
		// 	node = ft_lstnew(NULL);
		// 	node->type = CMD;
		// 	temp = lst->next->next;
		// 	lst->next->next = node;
		// 	node->next = temp;
		// }
		if (is_redirection(lst->type))
		{
			commands = 0;
			redirections++;
		}
		else if (lst->type == CMD)
			commands++;
		if (commands == redirections && !is_start && !is_operator(lst->type))
		{
			if (!lst->next || lst->next->type != CMD)
			{
				// printf("n : %s\n", lst->content);
				node = ft_lstnew(NULL);
				node->type = CMD;
				node->next = lst->next;
				lst->next = node;
			}
			redirections = 0;
		}
		if (lst->type == PIPE)
		{
			commands = 0;
			redirections = 0;
		}
		is_start = 0;
		lst = lst->next;
	}
}

t_tree	*build_tree(t_list	*tokens)
{
	t_list	*reversed_queue;
	t_list	*temp;
	t_queue *queue;
	t_tree *root;
	
	queue = build_sy_queue(tokens);
	reversed_queue = NULL;
	root = NULL;
	while (queue)
	{
		push(queue, &reversed_queue);
		queue = queue->next;
	}
	temp = reversed_queue; // to free it later;
	fill_cmd(reversed_queue);
	root = build_tree_from_rqueue(&reversed_queue);
	print_list(temp);
	print_tree(root, 0);
	return (root);
}
