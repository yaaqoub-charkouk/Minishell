#include "parsing.h"

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

t_queue	*new_node_op_queue(t_op	*operator) // FIFO stack // i assign type here 
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
// i create a t queue node from operator to queue it and from list .

void	add_to_queue(t_queue **queue, t_queue *new)// FIFO stack
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
} // i add an element to the back of the queue  because the queue is a fifo stack .

t_op	*new_operator_node(t_list	*token)
{
	t_op	*new_node;

	new_node = (t_op *)malloc(sizeof(t_op));
	if (!new_node)
		return (NULL);
	// new_node->content = token->content;
	new_node->content = ft_strdup(token->content);
	new_node->type = token->type;
	new_node->next = NULL;
	return (new_node);
}

int	push_to_op_stack(t_op **op_stack, t_list	*token) // push the operator to op stack
{
	t_op *new_node;

	// if (!*op_stack)
	// 		return (0);
	new_node = new_operator_node(token);
	new_node->next = *op_stack;
	*op_stack = new_node;
	return (1);
}

// t_queue	*build_sy_queue(t_list  *tokens)
// {
// 	t_list	*token;
// 	t_op	*op_stack; // the operator stack
// 	t_op	*operator;
// 	t_queue	*queue; 

// 	token = tokens;
// 	op_stack = NULL;
// 	queue = NULL;
// 	while (token)
// 	{
// 		if (token->type == CMD)
// 			add_back_queue(&queue, new_node_queue(token));
// 		else if (token->type != CMD)
// 		{
// 			// handle op case;
// 			if (token->type == P_OPEN)
// 			{
// 				while (token->type != P_CLOSE)
// 				{
// 					if (token->type == CMD)
// 						add_back_queue(&queue, new_node_queue(token));
// 					else
// 						push_to_operator(&op_stack, token);
// 					token = token->next;
// 				}
// 			}
// 			else if (token->type == P_CLOSE)
// 			{
// 				while (op_stack->type != P_OPEN)
// 				{
// 					add_back_queue(&queue, new_op_node_queue(op_stack));
// 					operator = op_stack;
// 					op_stack = op_stack->next;
// 					free(operator); // to avoid leaks;
// 				}
// 				operator = op_stack;
// 				op_stack = op_stack->next;
// 				free(operator);
// 			}
// 			else if (!op_stack || token->type > op_stack->type)
// 				push_to_operator(&op_stack, token);
// 			else if (token->type <= op_stack->type)
// 			{
// 				while (token->type <= op_stack->type)
// 				{
// 					add_back_queue(&queue, new_node_queue(token));
// 					token = token->next;
// 				}
// 				push_to_operator(&op_stack, token);
// 			}
// 		}
// 		token = token->next;
// 	}
// 	while (op_stack)
// 	{
// 		add_back_queue(&queue, new_op_node_queue(op_stack));
// 		operator = op_stack;
// 		op_stack = op_stack->next;
// 		free(operator);
// 	}
// 	return (queue);
// }