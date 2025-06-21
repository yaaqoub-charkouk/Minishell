/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharkho <akharkho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 21:55:18 by akharkho          #+#    #+#             */
/*   Updated: 2025/06/20 21:49:03 by akharkho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_ins.h"

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
