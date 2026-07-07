/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcompain <rcompain@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 12:00:27 by rcompain          #+#    #+#             */
/*   Updated: 2026/07/07 11:40:58 by rcompain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "00_Server.hpp"

/* ——— Static Variable —————————————————————————————————————————————————————— */
const int Server::_serverSocket = socket(AF_INET, SOCK_STREAM, 0);

/* ——— Constructor & Destructor ————————————————————————————————————————————— */
Server::Server(){
	//_commandsMap["PASS"] = fonction

	try{
		initServ();
	}
	catch (const std::runtime_error& error){
		std::cout << error.what() << std::endl;
		std::exit(-1);
	}
}



/* ——— Getters & Setters ———————————————————————————————————————————————————— */


/* ——— Methodes ————————————————————————————————————————————————————————————— */
void Server::initServ(){
	
	//AF_INET : IPv4 protocol
	//SOCK_STREAM: TCP socket
	this->_pollfds.push_back((struct pollfd){ _serverSocket, POLLIN, 0});
	this->_serverAddress.sin_family = AF_INET;
	this->_serverAddress.sin_port = htons(8000);
	std::cout << this->_serverAddress.sin_port << std::endl;
	this->_serverAddress.sin_addr.s_addr = INADDR_ANY;
	
	if (bind(_serverSocket, (struct sockaddr*)&_serverAddress, sizeof(_serverAddress)) < 0){
		close(_serverSocket);
		throw std::runtime_error("Bind failed.");
	}
		
	if (listen(_serverSocket, 5) < 0){ // 5 est la taille de la file d'attente
		close(_serverSocket);
		throw std::runtime_error("Listen failed.");
	}
		
	int opt = 1;
	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){
		close(_serverSocket);
		throw std::runtime_error("SetSockopt failed.");
	}
}