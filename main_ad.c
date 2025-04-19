/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_ad.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharkho <akharkho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 10:33:58 by akharkho          #+#    #+#             */
/*   Updated: 2025/04/18 14:24:50 by akharkho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_type_node	get_type(char *content)
{
	if (ft_strncmp(content, "|", 2) == 0)
		return (PIPE);
	else if (ft_strncmp(content, ">", 2) == 0)
		return (REDIRECTION_OUT);
	else if (ft_strncmp(content, "<", 2) == 0)
		return (REDIRECTION_IN);
	else if (ft_strncmp(content, ">>", 3) == 0)
		return (APPEND);
	else if (ft_strncmp(content, "<<", 3) == 0)
		return (HEREDOC);
	else if (ft_strncmp(content, "&&", 3) == 0)
		return (AND);
	else if (ft_strncmp(content, "||", 3) == 0)
		return (OR);
	else
		return (CMD);
}

t_list	*new_list(char *content)
{
	t_list	*new;
	
	new = malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->content = ft_strdup(content);
	if (!new->content)
	{
		free(new);
		return (NULL);
	}
	new->type = get_type(content);
	new->next = NULL;
	return (new);
}

void	free_split(char **tokens)
{
	int	i;
	
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
	tokens = NULL;
}

t_list	*organize_input(char *line)
{
	t_list	*list;
	t_list	*head;
	t_list	*new;
	char	**tokens;
	int		i;
	
	tokens = ft_split(line, ' ');
	if (!tokens)
		return (NULL);
	i = 0;
	while (tokens[i])
	{
		new = new_list(tokens[i]);
		if (!new)
		{
			free(tokens);
			return (NULL);
		}
		if (i == 0)
			head = new;
		else
			list->next = new;
		list = new;
		i++;
	}
	free_split(tokens);
	return (head);
}

t_tree	*create_tree(t_list *list)
{
	 t_tree	*tree_root;
	 
	 if (!list)
		 return (NULL);
	 tree_root = malloc(sizeof(t_tree));
	 if (!tree_root)
		 return (NULL);
	tree_root->cmd = NULL;
	tree_root->left_node = NULL;
	tree_root->right_node = NULL;
	if (list->type == CMD)
	{
		tree_root->type = CMD;
		tree_root->cmd = malloc(sizeof(char *) * 2);
		if (!tree_root->cmd)
			return (NULL);
		tree_root->cmd[0] = ft_strdup(list->content);
		tree_root->cmd[1] = NULL;
	}
	list = list->next;
	if (list)
	{
		if (list->type == PIPE || list->type == AND || list->type == OR)
		{
			tree_root->type = list->type;
			if (list->next)
				tree_root->left_node = create_tree(list->next);
			if (list->next->next)
				tree_root->right_node = create_tree(list->next->next);
		}
	}
	return (tree_root);
}

int	main(int ac, char **av, char **env)
{
	t_list	*list;
	t_tree	*tree;
	char	*line;
	
	(void)ac;
	(void)av;
	(void)env;
	list = NULL;
	tree = NULL;
	while (1)
	{
		line = readline("minishellghda> ");
		list = organize_input(line);
		tree = create_tree(list);
		(void)tree;
	}
	
	return (0);
}