/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcompain <rcompain@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 23:44:46 by rcompain          #+#    #+#             */
/*   Updated: 2026/07/08 00:12:43 by rcompain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signal.hpp"

volatile sig_atomic_t g_running = 1;

static void	handler(int sig)
{
	if (sig == SIGINT)
		g_running = 0;
}

void	init_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);

}