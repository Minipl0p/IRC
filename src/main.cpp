/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcompain <rcompain@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 11:57:26 by rcompain          #+#    #+#             */
/*   Updated: 2026/07/09 09:03:34 by rcompain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Server.hpp"
#include "signals/signal.hpp"
#include <fstream>

int main(int ac, char **av){
	
	if (ac != 3){
		std::cerr << "Invalid numbers of arguments." << std::endl;
		return -1;
	}
	
	init_signals();
	
	std::string password(av[2]);
	Server server(std::atoi(av[1]), password);
	const int POLL_TIMEOUT_MS = 3 * 60 * 1000;

	for(;g_running == 1;){

		// Check de poll sur fd du serveur
		if (poll(server.getLstFds().data() ,static_cast<nfds_t>(server.getLstFds().size()), POLL_TIMEOUT_MS) < 0){
			if (g_running == 1)
				std::cerr << "Poll failed." << std::endl;
			else
				std::cerr << std::endl;
			return 1;
		}

		if (server.getServerFd().revents & POLLIN)
			server.acceptNewClient();

		// Boucle de check de poll sur les autres fd
		for (std::vector<pollfd>::iterator it = server.getLstFds().begin() + 1; it != server.getLstFds().end();){
			if (it->revents & POLLIN)
				it = server.handleClientEvent(it);
			else
				++it;
		}
	}

	close(server.getServerSocket());
	
	return 0;
}