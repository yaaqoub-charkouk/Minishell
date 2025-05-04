/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharkho <akharkho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:22:55 by akharkho          #+#    #+#             */
/*   Updated: 2025/05/04 18:04:13 by akharkho         ###   ########.fr       */
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

void	ft_add_back(t_env **lst, t_env *new);
t_env	*ft_new(void *content);
char	*get_env_content(t_env *env, char *value);
void	copy_env(char **envp, t_env **env);
int		built_in_cd(char **args, t_env *env);
int		built_in_echo(char **args, t_env *env);
int		built_in_env(t_env *env);
int		built_in_exit(void);
int		built_in_pwd(void);
int		built_in_export(char **args, t_env **env);
int		built_in_unset(char **args, t_env **env);

#endif