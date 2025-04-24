#include "minishell.h"


// what i  need to do now is to fill the two stack operator and queue ;



void	add_back_queue(t_queue **op_stack, t_queue *new)// FIFO stack
{
	t_queue	*last;

	if (!op_stack || !new)
		return ;
	if (*op_stack == NULL)
	{
		*op_stack = new;
		return ;
	}
	last = *op_stack;
	while (last->next)
		last = last->next;
	last->next = new;
}

t_queue	*new_node_queue(t_list	*lst) // FIFO stack
{
	t_queue	*node;

	node = (t_queue *)malloc(sizeof(t_queue));
	if (!node)
		return (NULL);
	node->content = lst->content;
	node->type = lst->type; // i add the type to the node ;
	node->next = NULL;
	return (node);
}

t_tree  *build_tree(t_list  *tokens)
{
	t_list	*list;
	t_op	*op_stack;
	t_queue	*queue;

	while (list)
	{
		if (list->type == CMD)
		{
			// list->content   add back to operator stack; 
			add_back_queue(&op_stack, new_node_queue(list));
			// now i know the element type added to the queue .
		}
		else if (list->type != CMD)
		{
			// handle op case;
			
			// if the precedence of the current operator is higher than the operator on the top of the stack , add back all the operator to the queue

		}


	}
}
// i will create two stack , an operator stack LIFO , and a cmd or queue stack FIFO	 , then the result wich is the tree ,