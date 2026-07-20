/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcompain <rcompain@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 11:57:26 by rcompain          #+#    #+#             */
/*   Updated: 2026/07/18 17:20:31 by rcompain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Server.hpp"
#include "signals/signal.hpp"
#include <fstream>

#include "server/Server.hpp"
#include "signals/signal.hpp"

int main(int ac, char **av)
{
	if (ac != 3) {
		std::cerr << "Invalid numbers of arguments." << std::endl;
		return -1;
	}

	std::string password(av[2]);
	Server		server(std::atoi(av[1]), password);
	int			timeout = (3 * 60 * 1000);

	for (;;) {
		if (poll(server.getLstFds().data(),
				 static_cast<nfds_t>(server.getLstFds().size()),
				 timeout) < 0) {
			std::cerr << "Poll failed." << std::endl;
			close(server.getServerSocket());
			return 1;
		}

		if (server.getServerFd().revents & POLLIN) {
			server.acceptNewClient();
			continue;
		}

		for (std::vector<pollfd>::iterator it = server.getLstFds().begin() + 1;
			 it != server.getLstFds().end();) {
			if (it->revents & POLLIN)
				it = server.handleClientEvent(it);
			else
				++it;
		}
	}

	close(server.getServerSocket());
	return 0;
}
