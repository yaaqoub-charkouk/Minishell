// define how the tree will be , and build it .

#include "minishell.h"

int     push_op(t_stack **from, t_stack **to)
{
	t_stack *ptr;

	if (!*from)
			return (0);
	ptr = *from;
	*from = (*from)->next;
	ptr->next = *to;
	*to = ptr;
	return (1);
}