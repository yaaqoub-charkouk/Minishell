/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharkho <akharkho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:22:55 by akharkho          #+#    #+#             */
/*   Updated: 2025/06/13 17:06:46 by akharkho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_INS_H
# define BUILT_INS_H
# include "../utils/utils.h"



//export_utils.c
t_list	*get_env_value(t_list *env, char *value);
void	ft_put_variable(char *var);
int		is_unvalid_name(char *value);
char	*get_env_content(t_list *env, char *value);
void	copy_env(char **envp, t_list **env);
int		built_in_cd(char **args, t_data *data);
int		built_in_echo(char **args);
int		built_in_env(t_data *data);
int		built_in_exit(char **args, t_data *data);
int		built_in_pwd(void);
int		built_in_export(char **args, t_data *data);
int		built_in_unset(char **args, t_data *data);
char	**env_struct_to_char(t_list *env);

#endif