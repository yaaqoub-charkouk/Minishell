#include "signals.h"

void	sig_quit_child(int signal)
{
	(void)signal;
	write(2, "Quit: 3\n", 9);
}

void	reset_terminal_mode(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag |= (ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	write(STDERR_FILENO, "\r\033[K", 4);
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	display_new_prompt(int signal, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	(void)signal;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_sig = 1;
}

void	setup_signals(void )
{
	struct sigaction	action;
	struct sigaction	exit;

	reset_terminal_mode();
	action.sa_sigaction = display_new_prompt;
	action.sa_flags = SA_SIGINFO | SA_RESTART;
	sigemptyset(&action.sa_mask);
	sigaddset(&action.sa_mask, SIGINT);
	if (sigaction(SIGINT, &action, NULL) == -1)
		perror("sigaction ^C");
	exit.sa_handler = SIG_IGN;
	exit.sa_flags = SA_SIGINFO | SA_RESTART;
	sigemptyset(&exit.sa_mask);
	sigaddset(&exit.sa_mask, SIGQUIT);
	if (sigaction(SIGQUIT, &exit, NULL) == -1)
		perror("sigaction ^\\");
}
