#include "built_ins.h"

void	update_pwd(t_env *env, char *path, char *oldpwd)
{
	char	*new_pwd;
	char	*old_pwd;

	while (env)
	{
		if (ft_strncmp(env->content, "PWD=", 4) == 0)
		{
			new_pwd = ft_strjoin("PWD=", path);
			if (!new_pwd)
				return ;
			// free(env->content);			env->content = new_pwd;
		}
		else
		{
			// if (new_pwd)
			// {
			// 	char *join2 = ft_strjoin("PWD=", new_pwd);
			// 	built_in_export(&join2, &env);
			// }
		}
		if (ft_strncmp(env->content, "OLDPWD=", 7) == 0)
		{
			old_pwd = ft_strjoin("OLDPWD=", oldpwd);
			if (!old_pwd)
				return ;
			// free(env->content);
			env->content = old_pwd;
		}
		else
		{
			// if (old_pwd)
			// {
			// 	char *join = ft_strjoin("OLDPWD=", old_pwd);
			// 	built_in_export(&join, &env);
			// }
		}
		env = env->next;
	}
}

char	*get_env_content(t_env *env, char *value)
{
	int	len;
	
	len = 0;
	

	while (value[len] && ft_isalnum(value[len]))
	{
		len++;
	}
	while (env)
	{
		if (ft_strncmp(env->content, value, len) == 0 
			&& env->content[len] == '=')
			return (env->content + len + 1);
		env = env->next;
	}
	return (NULL);
}

int	built_in_cd(char **args, t_data *data)
{
	char	*path;
	char	*oldpwd;
	char	*prev_oldpwd;
	char	*newpwd;

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (perror("minishell: getcwd"), 1);
	if (!args[1] || !*args[1] || args[1][0] == '~')
	{
		path = get_env_content(*data->envl, "HOME");
		if (!path)
		{
			write(2, "cd: HOME not set\n", 17);
			return (free(oldpwd), 1);
		}
	}
	else if (args[1] && args[1][0] == '-')
	{
		prev_oldpwd = get_env_content(*data->envl, "OLDPWD");
		if (!prev_oldpwd)
		{
			write(2, "cd: OLDPWD not set\n", 19);
			return (free(oldpwd), 1);
		}
		printf("%s\n", prev_oldpwd);
		path = prev_oldpwd;
	}
	else
		path = args[1];
	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		return (free(oldpwd), 1);
	}
	newpwd = getcwd(NULL, 0);
	update_pwd(*data->envl, newpwd, oldpwd);
	return (free(oldpwd), free(newpwd), 0);
}
