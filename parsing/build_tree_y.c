#include "../minishell.h"

// t_tree	*help(t_list	*r_list, t_list	*right, t_list	*left)
// {
// 	t_tree *tree_root;

// 	tree_root = NULL;
// 	if (!right || !left)
// 		return (tree_root);
// 	tree_root = new_tree_node(r_list);
// 	if (right && right->type != CMD)
// 		tree_root->right = help(right, left, left->next);
// 	else
// 	{
// 		tree_root->right = new_tree_node(right);
// 		tree_root->left = new_tree_node(left); // in the case of one operator with two command
// 	}
// 	return (tree_root);
// }

t_list  *new_node_queue_list(t_queue *queue) // ,t_list *list)
{
	t_list	*node;

	node = (t_list *)malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	node->content = queue->content; // should i re alloc for the pointer or not 
	node->type = queue->type;
	node->next = NULL;
	// i need to free content here since it a return of a function so may be freeing the pointer here will be a good choice , and a good practice so yeah
	return (node);
}

int	push(t_queue *queue, t_list **list)
{
	t_list	*node;

	node = new_node_queue_list(queue);
	node->next = *list;
	*list = node;
	return (1);
}

void	print_list(t_list	*list)
{
	while (list)
	{
		printf ("%s ", list->content);
		list = list->next;
	}
	printf("\n");
}

void print_tree(t_tree *root) {
	if (root == NULL) {
		return;
	}
	
	printf("%s\n", root->cmd);
	
	print_tree(root->left);
	print_tree(root->right);
}


t_tree *new_tree_node(t_list	*token)
{
	t_tree	*tree_node;

	if (!token)
		return (NULL);
	tree_node = malloc(sizeof(t_tree));
	tree_node->args = ft_split(token->content, ' ');
	tree_node->cmd = token->content; // a copy from the first allocated token ;
	tree_node->type = token->type;
	tree_node->left = NULL;
	tree_node->right = NULL;
	return (tree_node);
}




t_tree *build_tree_from_rqueue(t_list **current)
{
	t_tree *node;

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





t_tree  *build_tree(t_list	*tokens)
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
	// root = build_tree_from_rqueue(&reversed_queue);
	root = build_tree_from_rqueue(&reversed_queue);
	// print_list(temp);
	print_tree(root);
	return (NULL);
}
