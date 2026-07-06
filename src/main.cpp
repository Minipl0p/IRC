/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcompain <rcompain@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 11:57:26 by rcompain          #+#    #+#             */
/*   Updated: 2026/07/06 17:12:06 by rcompain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/00_Server.hpp"
#include <unistd.h>
#include <fstream>

int main(){
	
	Server server;
	pollfd fdServer = server.getServerFd();

	for(;;){
		if (fdServer.revents & POLLIN){
			int newClientSocket = accept(server.getServerSocket(), NULL, NULL);
			server.getLstFds().push_back((struct pollfd){ newClientSocket, 0, 0}); // Mettre le bon flag
		}
	}

	close(server.getServerSocket());
	
	return 0;
}