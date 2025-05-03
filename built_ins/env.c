#include "built_ins.h"

t_env	*ft_new(void *content)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->content = content;
	node->next = NULL;
	// i need to free content here since it a return of a function so may be freeing the pointer here will be a good choice , and a good practice so yeah
	return (node);
}

void	ft_add_back(t_env **lst, t_env *new)
{
	t_env	*last;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last = *lst;
	while (last->next)
		last = last->next;
	last->next = new;
}

void	copy_env(char **envp, t_env **env)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		ft_add_back(env, ft_new(envp[i]));
		i++;
	}
}

int	built_in_env(t_env *env)
{
	while (env)
	{
		printf("%s\n", env->content);
		env = env->next;
	}
	return (0);
}
