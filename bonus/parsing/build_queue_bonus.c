/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_queue_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycharkou <ycharkou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 15:28:46 by ycharkou          #+#    #+#             */
/*   Updated: 2025/06/22 17:40:19 by ycharkou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing_bonus.h"

void	add_op_to_queue(t_list **queue, t_list **stack_op)
{
	t_list	*new_node;
	t_list	*operator;

	new_node = ft_lstnew((*stack_op)->content);
	if (!new_node)
		return ;
	new_node->type = (*stack_op)->type;
	ft_lstadd_back(queue, new_node);
	operator = *stack_op;
	*stack_op = (*stack_op)->next;
	free(operator);
}

int	push_to_op_stack(t_list **op_stack, t_list	*token)
{
	t_list	*new_node;

	new_node = ft_lstnew(token->content);
	if (!new_node)
		return (0);
	new_node->type = token->type;
	new_node->next = *op_stack;
	*op_stack = new_node;
	return (1);
}

void	parenthesis_priority(t_list **stack_op, t_list **queue)
{
	t_list	*temp;

	while (*stack_op && (*stack_op)->type != P_OPEN)
		add_op_to_queue(queue, stack_op);
	temp = *stack_op;
	*stack_op = (*stack_op)->next;
	free(temp);
}

int	precedence(t_type_node type)
{
	if (is_redirection(type))
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
	t_list	**to_free;

	queue = (((stack_op = NULL), to_free = &stack_op), NULL);
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
			while (stack_op && precedence(token->type)
				< precedence(stack_op->type))
				add_op_to_queue(&queue, &stack_op);
			push_to_op_stack(&stack_op, token);
		}
		token = token->next;
	}
	while (stack_op)
		add_op_to_queue(&queue, &stack_op);
	return (ft_lstclear(to_free, NULL), queue);
}
