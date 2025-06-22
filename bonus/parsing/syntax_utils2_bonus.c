/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils2_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycharkou <ycharkou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 18:27:45 by akharkho          #+#    #+#             */
/*   Updated: 2025/06/22 17:40:19 by ycharkou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing_bonus.h"

int	check_type_of_error(t_list *list)
{
	if (list->type == REDIRECTION_IN)
		return (print_error("<"), 1);
	if (list->type == HEREDOC)
		return (print_error("<<"), 1);
	if (list->type == REDIRECTION_OUT)
		return (print_error(">"), 1);
	if (list->type == APPEND)
		return (print_error(">>"), 1);
	if (list->type == AND)
		return (print_error("&&"), 1);
	if (list->type == OR)
		return (print_error("||"), 1);
	if (list->type == PIPE)
		return (print_error("|"), 1);
	return (0);
}

int	handle_redirections(t_list *list)
{
	while (list && list->next)
	{
		if ((list->type == REDIRECTION_IN || list->type == REDIRECTION_OUT
				|| list->type == APPEND || list->type == HEREDOC)
			&& list->next->type != CMD)
			return (check_type_of_error(list->next), 1);
		list = list->next;
	}
	if (check_type_of_error(list))
		return (1);
	return (0);
}
