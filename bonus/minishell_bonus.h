/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_bonus.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycharkou <ycharkou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 20:30:31 by akharkho          #+#    #+#             */
/*   Updated: 2025/06/22 16:40:18 by ycharkou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_BONUS_H
# define MINISHELL_BONUS_H
# include "built_ins/built_ins_bonus.h"
# include "execution/execution_bonus.h"
# include "parsing/parsing_bonus.h"
# include "signals/signals_bonus.h"
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
