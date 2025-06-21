/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycharkou <ycharkou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:54:02 by ycharkou          #+#    #+#             */
/*   Updated: 2025/06/21 15:20:02 by ycharkou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "../utils/utils.h"

char	**expand_wildcard(char *pattern);
char	*expand_heredoc(char *line, t_data *data, int should_expand_var);
char	**ft_expand(char *cmd, char **cmd_args,
			t_data *data, int *is_ambiguous);
char	*get_var_value(t_list *env, char *value, int *i, int *word_boundary);
char	*accumulate_char(char *pile, char c);
void	insert_variable(t_expand *expand, char **var, int space_flag, 
			int word_boundary);
int		ft_argslen(char **arg);
// utils
int		in_quotes(char c, int *in_dquotes, int *in_squotes, int *i);
char	*accumulate_char(char *pile, char c);
int		ft_argslen(char **arg);
int		should_expand_variable(t_expand *expand, int i);
void	expand_variable(t_data *data, t_expand *expand, int *i);
int		end_with_space(char *value);
char	**ignore_empty_args(char **args);

//command substitution
char	*expand_cmd_substitution(t_data *data, t_expand *expand, int *i);

#endif