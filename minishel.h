# ifndef MINISHEL_H
# define MINISHEL_H

#include <stdio.h>
# include "../includes/libft.h"
# include "../includes/ft_printf.h"
#include <readline/readline.h>
#include <readline/history.h>

typedef enum s_type
{
	CMD,
	PIPE,
	OR,
	AND,
	REDIRECTION_IN,
	REDIRECTION_OUT,
}	t_type;


#endif
