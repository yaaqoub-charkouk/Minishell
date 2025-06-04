#include "parsing.h"

void	add_token_to_queue(t_list **queue, t_list	*token)
{
	t_list	*new_node;

	new_node = ft_lstnew(token->content); // create a node a copy of the token with the same content and type ;
	if (!new_node)
		return ;
	new_node->type = token->type;
	// add_to_queue(queue, new_node);
	ft_lstadd_back(queue, new_node);
}

void	add_op_to_queue(t_list **queue, t_list **stack_op)
{
	t_list	*new_node;
	t_list	*operator;

	new_node = ft_lstnew((*stack_op)->content); // new node from stack_op head
	if (!new_node)
		return ;
	new_node->type = (*stack_op)->type;
	ft_lstadd_back(queue, new_node); // the head operator was added to queue;
	operator = *stack_op;
	*stack_op = (*stack_op)->next; // hte head has been changed to the next op ;
	free(operator);
}

int	push_to_op_stack(t_list **op_stack, t_list	*token) // push the operator to op stack
{
	t_list *new_node;

	// if (!*op_stack)
	// 		return (0);
	new_node = ft_lstnew(token->content);
	if (!new_node)
		return (0);
	new_node->type = token->type;
	new_node->next = *op_stack;
	*op_stack = new_node;
	return (1);
}

void	parenthesis_priority(t_list **stack_op, t_list  **queue)
{
	t_list *temp;

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

t_list	*build_sy_queue(t_list	*token)
{
	t_list	*queue;
	t_list	*stack_op;

	queue = NULL;
	stack_op = NULL;
	while (token)
	{
		if (token->type == CMD)
			add_token_to_queue(&queue, token); // done
		else if (token->type == P_OPEN)
			push_to_op_stack(&stack_op, token); // push
		else if (token->type == P_CLOSE)
			parenthesis_priority(&stack_op, &queue);
		else
		{
			while (stack_op && precedence(token->type) < precedence(stack_op->type))
				add_op_to_queue(&queue, &stack_op); // done
			push_to_op_stack(&stack_op, token); // done
		}
		token = token->next;
	}
	while (stack_op)
		add_op_to_queue(&queue, &stack_op); // done
	// free resourrces , token
	return (queue);
}
