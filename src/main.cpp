/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcompain <rcompain@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 11:57:26 by rcompain          #+#    #+#             */
/*   Updated: 2026/07/07 12:19:17 by rcompain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/00_Server.hpp"
#include <fstream>

int main(){
	
	Server server;
	pollfd fdServer = server.getServerFd();
	int timeout = (3 * 60 * 1000);

	for(;;){
		if (poll(&fdServer ,static_cast<nfds_t>(server.getLstFds().size()), timeout) < 0){
			std::cerr << "Poll failed." << std::endl;
			close(server.getServerSocket());
			return 1; 
		}
		if (fdServer.revents & POLLIN){
			int newClientSocket = accept(server.getServerSocket(), NULL, NULL);
			server.getLstFds().push_back((struct pollfd){ newClientSocket, 0, 0}); // Mettre le bon flag
		}
	}

	close(server.getServerSocket());
	
	return 0;
}