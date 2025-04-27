#include "minishell.h"

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

void	parenthesis_case(t_queue **queue, t_op **stack_op, t_list	**token)
{
	t_op	*operator;

	while ((*token)->type != P_CLOSE)
	{
		if ((*token)->type == CMD)
			add_token_to_queue(queue, *token);
		else if ((*token)->type != CMD)
			push_to_op_stack(stack_op, *token);
		*token = (*token)->next;
	}
	while((*stack_op)->type != P_OPEN) // there is at least one operator in the operator stack
	{	// push all operators to queue;
		add_op_to_queue(queue, stack_op);
	}
	operator = *stack_op;
	*stack_op = (*stack_op)->next;
	free(operator);
}

void	print_operator(t_op *operator)
{
	while (operator)
	{
		printf("%s\n", operator->content);
		operator = operator->next;
	}
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
		else if (token->type != CMD)
		{
			if (token->type == P_OPEN)
				parenthesis_case(&queue, &stack_op, &token);
			else if (!stack_op || token->type >= stack_op->type)
				push_to_op_stack(&stack_op, token); // push the operator to stack_op if the op stack is empty or the token precedence is higher than the stack  operator at the top of the stack 
			else if (token->type < stack_op->type)
			{
				// li f stack to queue
				while (!stack_op || token->type < stack_op->type)
				{
					add_op_to_queue(&queue, &stack_op);
				}
				push_to_op_stack(&stack_op, token);		
			}
			// nh
		}
		token = token->next;
	}
	// end of tokens
	while (stack_op)
	{
		add_op_to_queue(&queue, &stack_op);
	}
	// free resourrces , token
	
	return (queue);
}
