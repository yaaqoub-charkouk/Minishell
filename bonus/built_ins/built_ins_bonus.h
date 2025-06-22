/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_bonus.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycharkou <ycharkou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:22:55 by akharkho          #+#    #+#             */
/*   Updated: 2025/06/22 17:32:14 by ycharkou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_INS_BONUS_H
# define BUILT_INS_BONUS_H
# include "../utils/utils_bonus.h"

//export_utils.c
t_list	*get_env_value(t_list *env, char *value);
void	ft_put_variable(char *var);
int		is_unvalid_name(char *value, int is_unset);
char	*get_env_content(t_list *env, char *value);
void	copy_env(char **envp, t_list **env);
int		built_in_cd(char **args, t_data *data);
int		built_in_echo(char **args);
int		built_in_env(char **args, t_data *data);
int		built_in_exit(char **args, t_data *data);
int		built_in_pwd(t_data *data);
int		built_in_export(char **args, t_data *data);
int		built_in_unset(char **args, t_data *data);
char	**env_struct_to_char(t_list *env);
void	export_process_arg(char *arg, t_list **env, t_data *data);
#endif