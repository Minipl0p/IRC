/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcompain <rcompain@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 23:57:11 by rcompain          #+#    #+#             */
/*   Updated: 2026/07/08 00:12:36 by rcompain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <signal.h>

extern volatile sig_atomic_t g_running;

void init_signals(void);

void handler(int sig);
