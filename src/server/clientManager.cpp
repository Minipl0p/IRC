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
		return _pollfds.erase(it);

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

void Server::addClientsToLst(Client *src) { _listeningClientsMap[src->getFd()] = src; }

bool Server::findClientsToLst(fd &src) const
{
	std::map<fd, Client *>::const_iterator it = _listeningClientsMap.find(src);
	if (it == _listeningClientsMap.end())
		return false;
	return true;
}

void Server::deleteClientsToLst(Client *src)
{
	for (std::map<std::string, Channel *>::iterator it = _lstChannels.begin();
		 it != _lstChannels.end();) {
		Channel *chan = it->second;
		++it;
		if (chan->isMember(*src)) {
			chan->removeMember(*src);
			if (chan->isEmptyChannel())
				deleteChannelToLst(chan);
		}
	}
	_listeningClientsMap.erase(src->getFd());
	deletePollfdsToLst(src->getFd());
	delete src;
}

void Server::deleteClientsToLst(Client &src, std::vector<pollfd>::iterator &it)
{
	for (std::map<std::string, Channel *>::iterator cit = _lstChannels.begin();
		 cit != _lstChannels.end();) {
		Channel *chan = cit->second;
		++cit;
		if (chan->isMember(src)) {
			chan->removeMember(src);
			if (chan->isEmptyChannel())
				deleteChannelToLst(chan);
		}
	}
	_listeningClientsMap.erase(src.getFd());
	it = _pollfds.erase(it);
	close(src.getFd());
	delete &src;
}
