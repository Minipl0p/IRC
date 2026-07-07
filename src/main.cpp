/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcompain <rcompain@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 11:57:26 by rcompain          #+#    #+#             */
/*   Updated: 2026/07/07 15:36:44 by rcompain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/00_Server.hpp"
#include <fstream>

int main(int ac, char **av){
	
	if (ac != 3){
		std::cerr << "Invalid numbers of arguments." << std::endl;
		return -1;
	}
	
	std::string password(av[2]);
	Server server(std::atoi(av[1]), password);
	pollfd fdServer = server.getServerFd();
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
		}
	}

	close(server.getServerSocket());
	
	return 0;
}