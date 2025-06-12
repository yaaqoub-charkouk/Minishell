#include "parsing.h"

int	push(t_list *queue, t_list **list)
{
	t_list	*node;

	node = ft_lstnew(queue->content);
	if (!node)
		return (0);
	node->type = queue->type;
	node->next = *list;
	*list = node;
	return (1);
}

t_tree *new_tree_node(t_list	*token, t_data *data)
{
	t_tree	*tree_node;

	(void)data;
	if (!token)
		return (NULL);
	if (token->type == HEREDOC)
		data->is_heredoc++; // like semaphore;

	tree_node = malloc(sizeof(t_tree));
	tree_node->red.flag = 1; // pass a pointer to expand and check if we need to expand inside heredoc ;
	// tree_node->args = ft_split(token->content, ' '); // expand here
	if (token->type == CMD)
		tree_node->args = ft_split_pipex(token->content, ' ');   // wait every time you expand limiter;
	else
		tree_node->args = NULL;
	tree_node->cmd = token->content; // a copy from the first allocated token ;
	tree_node->type = token->type;
	tree_node->left = NULL;
	tree_node->right = NULL;
	tree_node->red.in_fd = -1;
	tree_node->red.outfile = NULL;
	tree_node->red.erno = 0;
	tree_node->red.file_name = NULL;
	return (tree_node);
}

t_tree	*build_tree_from_rqueue(t_list **current, t_data *data)
{
	t_tree	*node;

	if (!current || !*current)
		return (NULL);

	node = new_tree_node(*current, data);
	*current = (*current)->next;

	if (node->type != CMD)
	{
		node->right = build_tree_from_rqueue(current, data); // build the right first , so that the left will be the right->next ,wich will be updated when calling the function 
		node->left = build_tree_from_rqueue(current, data);
	}
	return (node);
}

t_tree	*build_tree(t_list	*tokens, t_data *data)
{
	t_tree	*root;
	t_list	*reversed_queue;
	t_list	*queue;
	t_list	*to_free;
	
	// print_list(tokens);
	queue = build_sy_queue(tokens); // don't allocate , just reorder tokens 
	// print_list(queue);
	reversed_queue = NULL;
	root = NULL;
	while (queue)
	{
		push(queue, &reversed_queue); // don't realloc 
		queue = queue->next;
	}
	to_free = reversed_queue; // to free it later;
	root = build_tree_from_rqueue(&reversed_queue, data);

	// print_tree(root, 0);
	return (root);
}
