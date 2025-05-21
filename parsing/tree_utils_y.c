#include "parsing.h"

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

void print_tree(t_tree *node, int level)
{
    if (!node)
        return;

    print_tree(node->right, level + 1);  // Print right subtree

    for (int i = 0; i < level; i++)
        printf("   ");  // Indentation
	if (node->args)
   		printf("%s\n", node->cmd);  // Print current node
	else
		printf("%s\n", NULL);
    print_tree(node->left, level + 1);  // Print left subtree
}

t_tree *new_tree_node(t_list	*token)
{
	t_tree	*tree_node;

	if (!token)
		return (NULL);
	tree_node = malloc(sizeof(t_tree));
	tree_node->args = ft_split(token->content, ' '); // expand here
	
	tree_node->cmd = token->content; // a copy from the first allocated token ;
	tree_node->type = token->type;
	tree_node->left = NULL;
	tree_node->right = NULL;
	tree_node->red.in_fd = -1;
	tree_node->red.outfile = NULL;
	return (tree_node);
}



