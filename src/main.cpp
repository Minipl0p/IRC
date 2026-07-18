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
#include <fstream>

int main(int ac, char **av){
	
	if (ac != 3){
		std::cerr << "Invalid numbers of arguments." << std::endl;
		return -1;
	}
	
	std::string password(av[2]);
	Server server(std::atoi(av[1]), password);
	const pollfd &fdServer = server.getServerFd();
	int timeout = (3 * 60 * 1000);

	for(;;){

		if (poll(server.getLstFds().data() ,static_cast<nfds_t>(server.getLstFds().size()), timeout) < 0){
			std::cerr << "Poll failed." << std::endl;
			close(server.getServerSocket());
			return 1; 
		}

		if (fdServer.revents & POLLIN){
			int newClientSocket = accept(server.getServerSocket(), NULL, NULL);
			if (newClientSocket < 0)
				break;
			server.getLstFds().push_back((struct pollfd){ newClientSocket, POLLIN, 0}); // Mettre le bon flag
			Client *newClient = new Client(newClientSocket);
			server.addClientsToLst(*newClient);
		}

		//lecture des clients
		for (size_t i = 1; i < server.getLstFds().size(); i++){
			pollfd &fdClient = server.getLstFds()[i];
			if (!(fdClient.revents & POLLIN))
				continue;
			Client *client = server.getListeningClientsMap()[fdClient.fd];
			char buff[4096];
			int n = recv(fdClient.fd, buff, sizeof(buff), 0);
			if (n <= 0){ // deconnexion du client
				std::string empty;
				server.deleteClientsToLst(client, empty);
				continue;
			}
			client->getReadBuffer().append(buff, n);
			server.handleClientData(*client);
		}
	}

	close(server.getServerSocket());
	
	return 0;
}