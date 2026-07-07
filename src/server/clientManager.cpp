/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcompain <rcompain@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 20:07:49 by rcompain          #+#    #+#             */
/*   Updated: 2026/07/07 20:57:19 by rcompain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::addClientsToLst(Client *newClient){
	this->_listeningClientsMap[newClient->getFd()] = newClient;
}

void	Server::deleteClientsToLst(Client & client, std::vector<pollfd>::iterator &it){
	close(client.getFd());

	// retire Le client de la liste des clients
	_listeningClientsMap.erase(client.getFd());

	// retire le client de la liste des fds
	it = _pollfds.erase(it);
	
	delete &client;
}