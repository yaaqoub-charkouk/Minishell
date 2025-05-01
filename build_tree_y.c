#include "minishell.h"

t_list  *new_node_queue_list(t_queue *queue, t_list *list)
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

int     push(t_queue *queue, t_list **list)
{
	t_list	*node;

	node = new_node_queue_list(queue, *list);
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

t_tree  *build_tree(t_queue *queue)
{
	t_list	*reversed_queue;

	reversed_queue = NULL;
	while (queue)
	{
		push(queue, &reversed_queue);
		queue = queue->next;
	}
	
	print_list(reversed_queue);
	return (NULL);	
}
