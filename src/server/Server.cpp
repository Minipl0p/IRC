/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcompain <rcompain@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 12:00:27 by rcompain          #+#    #+#             */
/*   Updated: 2026/07/06 15:55:45 by rcompain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "00_Server.hpp"

/* ——— Static Variable —————————————————————————————————————————————————————— */
const int Server::_serverSocket = socket(AF_INET, SOCK_STREAM, 0);

/* ——— Constructor & Destructor ————————————————————————————————————————————— */
Server::Server(){
	//_commandsMap["PASS"] = fonction

	initServ();
}



/* ——— Getters & Setters ———————————————————————————————————————————————————— */


/* ——— Methodes ————————————————————————————————————————————————————————————— */
void Server::initServ(){
	
	//AF_INET : IPv4 protocol
	//SOCK_STREAM: TCP socket
	this->_serverAddress.sin_family = AF_INET;
	this->_serverAddress.sin_port = htons(8000);
	this->_serverAddress.sin_addr.s_addr = INADDR_ANY;
	bind(_serverSocket, (struct sockaddr*)&_serverAddress, sizeof(_serverAddress));
	listen(_serverSocket, 5); // 5 est la taille de la file d'attente
}