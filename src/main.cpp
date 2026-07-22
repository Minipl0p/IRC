/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchazalm <pchazalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 11:57:26 by rcompain          #+#    #+#             */
/*   Updated: 2026/07/20 18:06:29 by pchazalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Server.hpp"
#include "signals/signal.hpp"
#include <fstream>

int main(int ac, char **av)
{
	if (ac != 3) {
		std::cerr << "Invalid numbers of arguments." << std::endl;
		return -1;
	}

	for (int i = 0; av[1][i]; i++) {
		if (!std::isdigit(av[1][i])) {
			std::cerr << "Error: port must be a number" << std::endl;
			return 1;
		}
	}

	int port = std::atoi(av[1]);
	if (port < 1024 || port > 65535) {
		std::cerr << "Error: port must be between 1024 and 65535" << std::endl;
		return 1;
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

		if (server.getLstFds()[0].revents & POLLIN) {
			int newClientSocket = accept(server.getServerSocket(), NULL, NULL);
			if (newClientSocket < 0)
				break;
			server.getLstFds().push_back(
				(struct pollfd){newClientSocket, POLLIN, 0}); // Mettre le bon flag
			Client *newClient = new Client(newClientSocket);
			server.addClientsToLst(*newClient);
			std::cout << "New Client :" << newClient->getNickname() << " " << newClient->getFd()
					  << std::endl;
		}

		// lecture des clients
		for (size_t i = 1; i < server.getLstFds().size(); i++) {
			pollfd &fdClient = server.getLstFds()[i];
			if (!(fdClient.revents & POLLIN))
				continue;
			Client *client = server.getListeningClientsMap()[fdClient.fd];
			char	buff[4096];
			int		n = recv(fdClient.fd, buff, sizeof(buff), 0);
			if (n <= 0) { // deconnexion du client
				std::string quitMsg = ":" + client->getNickname() + "!" + client->getUsername() +
									  "@localhost QUIT :Connection closed\r\n";
				server.deleteClientsToLst(client, quitMsg);
				--i;
				continue;
			}
			client->getReadBuffer().append(buff, n);
			server.handleClientData(*client);
		}
	}
	close(server.getServerSocket());

	return 0;
}
