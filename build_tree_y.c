#include "minishell.h"


// what i  need to do now is to fill the two stack operator and queue ;

t_queue	*new_node_queue(t_list	*lst) // FIFO stack // i assign type here 
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

t_queue	*new_op_node_queue(t_op	*operator) // FIFO stack // i assign type here 
{
	t_queue	*node;

	node = (t_queue *)malloc(sizeof(t_queue));
	if (!node)
		return (NULL);
	node->content = operator->content;
	node->type = operator->type; // i add the type to the node ;
	node->next = NULL;
	return (node);
}

void	add_back_queue(t_queue **queue, t_queue *new)// FIFO stack
{
	t_queue	*last;

	if (!queue || !new)
		return ;
	if (*queue == NULL)
	{
		*queue = new;
		return ;
	}
	last = *queue;
	while (last->next)
		last = last->next;
	last->next = new;
}

int	push_to_operator(t_op **op_stack, t_list	*token) // push the operator to op stack
{
	t_op *new_node;

	if (!*op_stack)
			return (0);
	new_node = malloc(sizeof(t_op));
	new_node->content = token->content;
	new_node->next = *op_stack;
	new_node->type = token->type;
	*op_stack = new_node;
	return (1);
}

t_queue  *build_sy_queue(t_list  *tokens)
{
	t_list	*list;
	t_op	*op_stack; // the operator stack
	t_queue	*queue; 
	t_op	*operator;

	list = tokens;
	op_stack = NULL;
	queue = NULL;
	while (list)
	{
		if (list->type == CMD)
		{
			add_back_queue(&queue, new_node_queue(list));
		}
		else if (list->type != CMD)
		{
			// handle op case;
			if (list->type == P_OPEN)
			{
				while (list->type != P_CLOSE)
				{
					if (list->type == CMD)
						add_back_queue(&queue, new_node_queue(list));
					else
						push_to_operator(&op_stack, list);
					list = list->next;
				}
			}
			else if (list->type == P_CLOSE)
			{
				while (op_stack->type != P_OPEN)
				{
					add_back_queue(&queue, new_op_node_queue(op_stack));
					operator = op_stack;
					op_stack = op_stack->next;
					free(operator); // to avoid leaks;
				}
				operator = op_stack;
				op_stack = op_stack->next;
				free(operator);
			}
			else if (!op_stack || list->type > op_stack->type)
				push_to_operator(&op_stack, list);
			else if (list->type <= op_stack->type)
			{
				while (list->type <= op_stack->type)
				{
					add_back_queue(&queue, new_node_queue(list));
					list = list->next;
				}
				push_to_operator(&op_stack, list);
			}
		}
		list = list->next;
	}
	while (op_stack)
	{
		add_back_queue(&queue, new_op_node_queue(op_stack));
		operator = op_stack;
		op_stack = op_stack->next;
		free(operator);
	}
	return (queue);
}
// i will create two stack , an operator stack LIFO , and a cmd or queue stack FIFO	 , then the result wich is the tree ,