/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cmds_a.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharkho <akharkho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 14:14:00 by akharkho          #+#    #+#             */
/*   Updated: 2025/04/27 18:20:01 by akharkho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	built_in_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("pwd");
}

void	built_in_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
}

int	built_in_cd(char **args, char **envp)
{
	char	*path;
	char	*oldpwd;

	if (!args[1] || !*args[1])
	{
		path = getenv("HOME");
		if (!path)
		{
			write(2, "cd: HOME not set\n", 17);
			return (1);
		}
	}
	// else if (args[1] && args[1][0] == '-')//dont know if we need to handel this case
	// {
	// 	oldpwd = getenv("OLDPWD");
	// 	if (!oldpwd)
	// 	{
	// 		write(2, "cd: OLDPWD not set\n", 19);
	// 		return (1);
	// 	}
	// 	printf("%s", oldpwd);
	// }
	else
		path = args[1];
	if (chdir(path) != 0)
		perror("minishell: cd");
}

int	built_in_exit(void)
{
	printf("exit\n");
	exit(0);
}

int	built_in_unset(char *var, char **envp)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	j = 0;
	len = ft_strlen(var);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var, len) == 0 && envp[i][len] == '=')
		{
			j = i;
			while (envp[j])
			{
				envp[j] = envp[j + 1];
				j++;
			}
			break ;
		}
		i++;
	}
}

int	built_in_echo(char **args, char **envp)
{
	int	nl_flag;
	int	i;

	i = 1;
	nl_flag = 0;
	if (ft_strncmp(args[i], "-n", 3) == 0)
	{
		nl_flag = 1;
		i++;
	}
	if (!args[i])
	{
		printf("\n");
		return (0);
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
	}
	if (nl_flag == 0)
		printf("\n");
	return (0);
}
