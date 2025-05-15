#include "../../includes/libft.h"

int	ft_lstsize(t_list *lst)
{
	int	c;

	if (!lst)
		return (0);
	c = 0;
	while (lst)
	{
		lst = lst->next;
		c++;
	}
	return (c);
}
