/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcompain <rcompain@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 11:57:26 by rcompain          #+#    #+#             */
/*   Updated: 2026/07/07 21:09:01 by rcompain         ###   ########.fr       */
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
	int timeout = (3 * 60 * 1000);

	for(;;){

		// Check de poll sur fd du serveur
		if (poll(server.getLstFds().data() ,static_cast<nfds_t>(server.getLstFds().size()), timeout) < 0){
			std::cerr << "Poll failed." << std::endl;
			close(server.getServerSocket());
			return 1; 
		}
		if (server.getServerFd().revents & POLLIN){
			int newClientSocket = accept(server.getServerSocket(), NULL, NULL);
			if (newClientSocket < 0)
				continue;
			server.getLstFds().push_back((struct pollfd){ newClientSocket, POLLIN, 0}); // Mettre le bon flag
			server.addClientsToLst(new Client(newClientSocket)); // Rempli la liste de clients (a destroy)
		}

		// Boucle de check de poll sur les autre fd
		for (std::vector<pollfd>::iterator it = server.getLstFds().begin() + 1; it != server.getLstFds().end(); it++){
			
			// Check revents sur CLient
			if ((*it).revents & POLLIN){
				char buf[4096];
				int fdClient = (*it).fd;
				
				int res = recv(fdClient, &buf, sizeof(buf), 0);
				
				if (res <= 0){ // Deconection du client
					server.deleteClientsToLst(*(server.getListeningClientsMap().find(fdClient)->second), it);
					it--; // it-- pour compenser le it++ du for car it = erase() dans deleteClientsToLst()
					continue;
				}

				std::string &readBuffer = server.getListeningClientsMap()[(*it).fd]->getReadBuffer();
				readBuffer += std::string(buf, res);
				server.handleClientData(*(server.getListeningClientsMap()[(*it).fd]));
			}
		}
	}

	close(server.getServerSocket());
	
	return 0;
}