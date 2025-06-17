/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tree_y.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycharkou <ycharkou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 18:50:42 by ycharkou          #+#    #+#             */
/*   Updated: 2025/06/17 18:50:45 by ycharkou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	add_token_to_queue(t_list **queue, t_list	*token)
{
	t_list	*new_node;

	new_node = ft_lstnew(token->content);
	if (!new_node)
		return ;
	new_node->type = token->type;
	ft_lstadd_back(queue, new_node);
}

int	push(t_list *queue, t_list **list)
{
	t_list	*node;

	node = ft_lstnew(queue->content);
	if (!node)
		return (0);
	node->type = queue->type;
	node->next = *list;
	*list = node;
	return (1);
}

t_tree	*new_tree_node(t_list	*token, t_data *data)
{
	t_tree	*tree_node;

	(void)data;
	if (!token)
		return (NULL);
	tree_node = malloc(sizeof(t_tree));
	if (!tree_node)
		return (NULL);
	tree_node->red.flag = 1;
	if (token->type == CMD)
		tree_node->args = ft_split_pipex(token->content, ' ');
	else
		tree_node->args = NULL;
	free(token->content);
	token->content = NULL;
	tree_node->type = token->type;
	free(token);
	tree_node->left = NULL;
	tree_node->right = NULL;
	tree_node->red.in_fd = -1;
	tree_node->red.outfile = NULL;
	tree_node->red.erno = 0;
	tree_node->red.file_name = NULL;
	return (tree_node);
}

t_tree	*build_tree_from_rqueue(t_list **current, t_data *data)
{
	t_tree	*node;
	t_list	*temp;

	if (!current || !*current)
		return (NULL);
	temp = (*current)->next;
	node = new_tree_node(*current, data);
	*current = temp;
	if (node->type != CMD)
	{
		node->right = build_tree_from_rqueue(current, data);
		node->left = build_tree_from_rqueue(current, data);
	}
	return (node);
}

t_tree	*build_tree(t_list	*tokens, t_data *data)
{
	t_tree	*root;
	t_list	*reversed_queue;
	t_list	*queue;
	t_list	*to_free;

	queue = build_sy_queue(tokens);
	ft_lstclear(&tokens, NULL);
	to_free = queue;
	reversed_queue = NULL;
	root = NULL;
	while (queue)
	{
		push(queue, &reversed_queue);
		queue = queue->next;
	}
	ft_lstclear(&to_free, NULL);
	root = build_tree_from_rqueue(&reversed_queue, data);
	ft_lstclear(&reversed_queue, NULL);
	to_free = NULL;
	return (root);
}
