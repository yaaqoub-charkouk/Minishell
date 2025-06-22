/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycharkou <ycharkou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 20:30:31 by akharkho          #+#    #+#             */
/*   Updated: 2025/06/22 16:39:48 by ycharkou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "built_ins/built_ins.h"
# include "execution/execution.h"
# include "parsing/parsing.h"
# include "signals/signals.h"
# include <stdlib.h>
# define GREEN       "\001\033[32m↪  \002"
# define SKY_BLUE    "\001\033[1;36m\002"
# define RED     "\001\033[1;31m↪  \002"
# define RESET_COLOR "\001\033[0m\002"

// main_utils

void	pad_redirections_with_cmd(t_list **lst);
int		build_execute(t_list *tokens, t_data *data);
int		tokenize_syntax(t_data *data, t_list **tokens, char *line);
void	initialise_vars(t_data *data, t_list **env,
			t_list **tokens, char **envp);

#endif
