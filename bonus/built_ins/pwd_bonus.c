/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycharkou <ycharkou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 21:55:18 by akharkho          #+#    #+#             */
/*   Updated: 2025/06/22 11:23:44 by ycharkou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_ins_bonus.h"

int	built_in_pwd(t_data *data)
{
	char	*cwd;

	if (*data->envl)
		cwd = ft_strdup(get_env_content(*data->envl, "PWD"));
	else 
		cwd = getcwd(NULL, 0);
	if (cwd != NULL)
		printf("%s\n", cwd);
	else
		perror("minishell :pwd: ");
	free(cwd);
	return (0);
}
