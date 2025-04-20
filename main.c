/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharkho <akharkho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 10:33:58 by akharkho          #+#    #+#             */
/*   Updated: 2025/04/20 12:59:19 by akharkho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		new = new_node(tokens[i]);
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
	free_matrix(tokens);
	return (head);
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
		line = readline("minishell-1.0 $");
		add_history(line);
		if (is_syntax_error(line))
			continue ;
		
		// list = organize_input(line);
		// tree = create_tree(list);
		(void)tree;
	}
	
	return (0);
}