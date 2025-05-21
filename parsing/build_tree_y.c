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
	root = build_tree_from_rqueue(&reversed_queue);
	print_list(temp);
	print_tree(root, 0);
	return (root);
}
