# ifndef MINISHELL_H
# define MINISHELL_H
# include "built_ins/built_ins.h"
# include "parsing/parsing.h"
# include "execution/execution.h"
# include "signals/signals.h"
#include <stdlib.h>
// extern int g_sig;
typedef struct s_wild
{
	char			*file;
	struct s_wild	*next;
}	t_wild;

#endif
