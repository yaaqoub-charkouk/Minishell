#ifndef SIGNALS_H
# define SIGNALS_H

# include "../utils/utils.h"
# include <signal.h>
# include <stddef.h>
# include <sys/termios.h>

void	setup_signals(void );
void	display_new_prompt(int signal, siginfo_t *info, void *context);
void	reset_terminal_mode(void);
void	sig_quit_child(int signal);

#endif