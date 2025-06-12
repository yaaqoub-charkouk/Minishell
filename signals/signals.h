#ifndef SIGNALS_H
# define SIGNALS_H

#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
# include "../minishell.h"
#include "../parsing/parsing.h"
#include <sys/termios.h>

void	setup_signals(void );
void    reset_terminal_mode(void);

#endif