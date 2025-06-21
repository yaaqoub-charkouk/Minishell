/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycharkou <ycharkou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 20:28:17 by ycharkou          #+#    #+#             */
/*   Updated: 2025/06/21 20:28:19 by ycharkou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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