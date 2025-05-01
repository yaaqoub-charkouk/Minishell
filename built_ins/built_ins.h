/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharkho <akharkho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:22:55 by akharkho          #+#    #+#             */
/*   Updated: 2025/05/01 17:11:19 by akharkho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_INS_H
# define BUILT_INS_H
# include "../includes/libft.h" 
# include <stdio.h>

typedef struct s_env
{
	char			*content;
	struct s_env	*next;
}	t_env;
char	*get_env_content(t_env *env, char *value);
void	copy_env(char **envp, t_env **env);
int		built_in_cd(char **args, t_env *env);
int		built_in_echo(char **args, t_env *env);
int		built_in_env(t_env *env);
int		built_in_exit(void);
int		built_in_pwd(void);
int		built_in_unset(char *var, char **envp);

#endif