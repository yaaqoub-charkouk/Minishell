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


int	is_redirection(t_type_node type)
{
	return (type == REDIRECTION_IN || type == REDIRECTION_OUT 
			|| type == APPEND || type == HEREDOC);
}

int	is_operator(t_type_node type)
{
	return (type == PIPE || type == OR || type == AND);
}
