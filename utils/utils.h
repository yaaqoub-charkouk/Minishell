#ifndef UTILS_H
# define UTILS_H

# include "struct.h"
# include "../signals/signals.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <fcntl.h>
# include <errno.h>
# include <dirent.h>
# include <sys/wait.h>

//utils.c
void	close_read_fd(t_tree *node);
int		ft_strcmp(const char *s1, const char *s2);
char	**free_strs(char **strs, int i);
int		check_sep(char c, char sep);
int		signal_exit(t_tree *node, int is_pipe);

// free
void	free_tree(t_tree *tree);
void	free_matrix(char **args);
void	free_env(char **env, t_list **envl);
char	**ft_split_pipex(const char *s, char c);

// redirections utils
char	**list_to_char(t_list *args_list);
t_list	*add_cmd_options(t_list **args_list, char **args, int i);
int		is_redirection(t_type_node type);
int		is_operator(t_type_node type);
int		get_operator_len(t_type_node type);
int		check_ambiguity(t_redir *redir, char *file_name, int is_ambiguous);

// parsing_utils
void	handle_quotes(char c, int *in_squote, int *in_dquote);
int		is_space(char c);
void	print_error(char *token);
int		operator_error(int count, int flag);

#endif