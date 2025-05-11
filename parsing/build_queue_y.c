#include "parsing.h"

void	add_token_to_queue(t_queue **queue, t_list	*token)
{
	t_queue	*new_node;

	new_node = new_node_queue(token); // create a node a copy of the token with the same content and type ;
	if (!new_node)
		return ;
	add_to_queue(queue, new_node);
}

void	add_op_to_queue(t_queue **queue, t_op **stack_op)
{
	t_queue	*new_node;
	t_op	*operator;

	new_node = new_node_op_queue(*stack_op); // queue , copy of operator;
	if (!new_node)
		return ;
	add_to_queue(queue, new_node); // the head operator was added to queue;
	operator = *stack_op;
	*stack_op = (*stack_op)->next; // hte head has been changed to the next op ;
	free(operator);
}


void	parenthesis_priority(t_op **stack_op, t_queue  **queue)
{
	t_op *temp;

	while (*stack_op && (*stack_op)->type != P_OPEN)
	{
		add_op_to_queue(queue, stack_op);
	}
	temp = *stack_op; // the P_OPEN 
	*stack_op = (*stack_op)->next;
	free(temp);
}

int	precedence(t_type_node type) // the precedent make the position of the operator in the tree 
{
	if (type == HEREDOC || type == APPEND || type == REDIRECTION_OUT || type == REDIRECTION_IN)
		return (3);
	else if (type == PIPE)
		return (2);
	else if (type == OR || type == AND)
		return (1);
	return (0);
}

t_queue	*build_sy_queue(t_list	*token)
{
	t_queue	*queue;
	t_op	*stack_op;

	queue = NULL;
	stack_op = NULL;
	while (token)
	{
		if (token->type == CMD)
			add_token_to_queue(&queue, token);
		else if (token->type == P_OPEN)
			push_to_op_stack(&stack_op, token);
		else if (token->type == P_CLOSE)
			parenthesis_priority(&stack_op, &queue);
		else
		{
			while (stack_op && precedence(token->type) < precedence(stack_op->type))
				add_op_to_queue(&queue, &stack_op);
			push_to_op_stack(&stack_op, token);
		}
		token = token->next;
	}
	while (stack_op)
		add_op_to_queue(&queue, &stack_op);
	// free resourrces , token
	return (queue);
}
