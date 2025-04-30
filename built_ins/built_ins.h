/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharkho <akharkho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:22:55 by akharkho          #+#    #+#             */
/*   Updated: 2025/04/30 15:48:21 by akharkho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_INS_H
# define BUILT_INS_H
# include "../includes/libft.h" 
# include <stdio.h>

int	built_in_cd(char **args, char **envp);
int	built_in_echo(char **args, char **envp);
int	built_in_env(char **envp);
int	built_in_exit(void);
int	built_in_pwd(void);
int	built_in_unset(char *var, char **envp);

#endif