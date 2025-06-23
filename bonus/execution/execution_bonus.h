/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_bonus.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharkho <akharkho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 20:33:17 by akharkho          #+#    #+#             */
/*   Updated: 2025/06/23 13:00:16 by akharkho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_BONUS_H
# define EXECUTION_BONUS_H

# include "../expand/expand_bonus.h"
# include "../built_ins/built_ins_bonus.h"

//execute_utils
char	**get_path(char **env, int *erno);
void	exec_cmd_from_path(char **path, char *cmd, char **args, char **env);
void	check_access_err(t_tree *node, char **env);
void	exec_cmd(t_tree *node, char **env);
int		exit_status(int status);
int		check_if_directory(char	*cmd, char *direct, int should_print);
// execute command
char	**get_path(char **env, int *erno);
void	exec_cmd_from_path(char **path, char *cmd, char **args, char **env);
int		execute_built_in(char **args, t_data *data, int is_pipe, t_tree *node);
void	exec_cmd(t_tree *node, char **env);
int		execute_cmd(t_tree *node, t_data *data, int is_pipe);
int		execute_or(t_tree *node, t_data *data, int is_pipe);
int		execute_and(t_tree *node, t_data *data, int is_pipe);
int		execute_pipe(t_tree *node, t_data *data);
int		execution(t_tree *node, t_data *data, int is_pipe);
// redirections
int		pre_execution(t_tree *node, t_data *data);
void	open_heredoc(t_data *data, t_tree *node, t_redir *redir);
void	open_infile(char *filename,	t_redir	*redir);
void	open_outfile(char	*filename, t_redir *redir);

#endif