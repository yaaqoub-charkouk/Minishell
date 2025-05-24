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

	len = ft_strlen(value);
	while (env)
	{
		if (ft_strncmp(env->content, value, len) == 0 
			&& env->content[len] == '=')
			return (env->content + len + 1);
		env = env->next;
	}
	return (NULL);
}

int	built_in_cd(char **args, t_env *env)
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
		path = get_env_content(env, "HOME");
		if (!path)
		{
			write(2, "cd: HOME not set\n", 17);
			return (free(oldpwd), 1);
		}
	}
	else if (args[1] && args[1][0] == '-')
	{
		prev_oldpwd = get_env_content(env, "OLDPWD");
		if (!prev_oldpwd)
		{
			write(2, "cd: OLDPWD not set\n", 19);
			return (free(oldpwd), 1);
		}
		printf("%s\n", prev_oldpwd);
		path = prev_oldpwd;
	}
	else if (args[1][0] == '\"' || args[1][0] == '$')
	{
		char *special;
		while (*args[1] == '\"')
			args[1]++;
		special = args[1];
		while (*args[1] && *args[1] != '\"')
			args[1]++;
		*args[1] = '\0';
		if (!special)
			return (free(oldpwd), 1);
		if (*special == '$')
			special++;
		path = get_env_content(env, special);
		if (!path)
			return (write(2, "cd: ", 4), write(2, special, ft_strlen(special)),
				write(2, " not set\n", 9), free(oldpwd), 1);
	}
	else
		path = args[1];
	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		return (free(oldpwd), 1);
	}
	newpwd = getcwd(NULL, 0);
	update_pwd(env, newpwd, oldpwd);
	return (free(oldpwd), free(newpwd), 0);
}
