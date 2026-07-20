/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcompain <rcompain@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 20:07:49 by rcompain          #+#    #+#             */
/*   Updated: 2026/07/09 09:09:04 by rcompain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <vector>

void Server::acceptNewClient()
{
	int newClientSocket = accept(_serverSocket, NULL, NULL);
	if (newClientSocket < 0) {
		std::cerr << "Accept failed." << std::endl;
		return;
	}

	fcntl(newClientSocket, F_SETFL, O_NONBLOCK);

	_pollfds.push_back((struct pollfd){newClientSocket, POLLIN, 0});
	addClientsToLst(new Client(newClientSocket));
}

std::vector<pollfd>::iterator Server::handleClientEvent(std::vector<pollfd>::iterator it)
{
	static const size_t BUFFER_SIZE = 4096;
	char				buf[BUFFER_SIZE];
	int					fdClient = it->fd;

	std::map<fd, Client *>::iterator clientIt = _listeningClientsMap.find(fdClient);
	if (clientIt == _listeningClientsMap.end())
		return _pollfds.erase(it); // ne devrait jamais arriver si addClientsToLst est ok

	Client *client = clientIt->second;
	int		res	   = recv(fdClient, buf, BUFFER_SIZE, 0);

	if (res <= 0) {
		deleteClientsToLst(*client, it);
		return it;
	}

	client->getReadBuffer() += std::string(buf, res);
	handleClientData(*client);
	return ++it;
}
