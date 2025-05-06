#include "signals.h"


void	display_new_prompt(int signal, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	(void)signal;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	do_nothing(int signal, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	(void)signal;
	// write(1, "ctrl^\\ does nothing\n", 21);
}

void	setup_signals(void )
{
	struct sigaction	action;
	struct sigaction	exit;


	// setup action signal set
	action.sa_sigaction = display_new_prompt;
	action.sa_flags = SA_SIGINFO | SA_RESTART;
	sigemptyset(&action.sa_mask);
	sigaddset(&action.sa_mask, SIGINT); // there is only one signal
	if(sigaction(SIGINT, &action, NULL) == -1)
		perror("sigaction ^C");

	/*============================================*/
	// setup exit signal set // neeed to be removed 
	exit.sa_sigaction = do_nothing;
	exit.sa_flags = SA_SIGINFO | SA_RESTART;
	sigemptyset(&exit.sa_mask);
	sigaddset(&exit.sa_mask, SIGQUIT);
	
	// signal(SIGQUIT, SIG_IGN);
	if (sigaction(SIGQUIT, &exit, NULL) == -1)
		perror("sigaction ^\\");
}