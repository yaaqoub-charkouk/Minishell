#ifndef SIGNALS_H
# define SIGNALS_H

#include "../utils/utils.h"
#include <signal.h>
#include <stddef.h>
#include <sys/termios.h>

void	setup_signals(void );
void    reset_terminal_mode(void);

#endif