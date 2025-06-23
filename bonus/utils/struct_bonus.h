/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharkho <akharkho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 20:28:47 by akharkho          #+#    #+#             */
/*   Updated: 2025/06/23 13:00:36 by akharkho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_BONUS_H
# define STRUCT_BONUS_H
# include "../../libft/libft.h"

extern int	g_sig;

typedef struct s_io_config
{
	char	*outfile;
	int		in_fd;
	int		flag;
	int		erno;
	char	*file_name;
}	t_io_config;

typedef struct s_tree
{
	char			*cmd;
	char			**args;
	t_type_node		type;
	t_io_config		red;
	struct s_tree	*right;
	struct s_tree	*left;
}	t_tree;

typedef struct s_tokenize
{
	int			i;
	int			start;
	t_list		**head;
	t_list		**last;
	t_type_node	type;
}	t_tokenize;

typedef struct s_expand
{
	char	***args;
	char	***temp;
	char	*arg;
	char	**pile;
	char	**var_value;
	int		*k;
	int		in_dquotes;
	int		in_squotes;
	int		*is_ambiguous;
	int		space_flag;
	int		word_boundary;
	int		j;
}	t_expand;

typedef struct s_redir
{
	t_tree		*node;
	t_tree		*entry_node;
	t_type_node	*type;
	t_list		*args_list;
	int			open_error;
}	t_redir;

typedef struct s_data
{
	t_list	**envl;
	char	**env;
	int		read_fd;
	int		exit_status;
	int		is_heredoc;
	int		signaled;
	int		cmd_sub;
}	t_data;

#endif