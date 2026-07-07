/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcompain <rcompain@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 12:00:27 by rcompain          #+#    #+#             */
/*   Updated: 2026/07/07 20:18:35 by rcompain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* ——— Constructor & Destructor ————————————————————————————————————————————— */
Server::Server(int port, std::string password){
	//_commandsMap["PASS"] = fonction

	try{
		initServ(port, password);
	}
	catch (const std::runtime_error& error){
		std::cout << error.what() << std::endl;
		std::exit(-1);
	}

	initCommands();
}

Server::~Server() {
	// Destroy lst client
}

/* ——— Getters & Setters ———————————————————————————————————————————————————— */
std::vector<pollfd>& Server::getLstFds() { return _pollfds;}
const std::map<std::string, Command>& Server::getCommandsMap() { return _commandsMap;}
std::map<fd, Client*>& Server::getListeningClientsMap() { return _listeningClientsMap;}
const int& Server::getServerSocket() const { return _serverSocket;}
const sockaddr_in& Server::getServerAddress() const {return _serverAddress;}
const pollfd& Server::getServerFd() const {return _pollfds[0];}
		
/* ——— Functions         ———————————————————————————————————————————————————— */
void Server::initServ(int port, std::string password){
	
	//AF_INET : IPv4 protocol
	//SOCK_STREAM: TCP socket
	this->_password = password;
	this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket < 0){
		throw std::runtime_error("ServerSocket failed.");
	}
	this->_pollfds.push_back((struct pollfd){ _serverSocket, POLLIN, 0});
	this->_serverAddress.sin_family = AF_INET;
	this->_serverAddress.sin_port = htons(port);
	std::cout << this->_serverAddress.sin_port << std::endl;
	this->_serverAddress.sin_addr.s_addr = INADDR_ANY;
	
	int opt = 1;
	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){
		close(_serverSocket);
		throw std::runtime_error("SetSockopt failed.");
	}

	if (bind(_serverSocket, (struct sockaddr*)&_serverAddress, sizeof(_serverAddress)) < 0){
		close(_serverSocket);
		if (errno == EADDRINUSE)
			throw std::runtime_error("Port already used.");
		else
			throw std::runtime_error("Bind failed.");
	}
		
	if (listen(_serverSocket, 5) < 0){ // 5 est la taille de la file d'attente
		close(_serverSocket);
		throw std::runtime_error("Listen failed.");
	}
}

void Server::handleClientData(Client &client)
{
	std::string &buf = client.getReadBuffer();
	size_t		 pos;

	while ((pos = buf.find("\r\n")) != std::string::npos) {
		std::string line = buf.substr(0, pos);
		buf.erase(0, pos + 2);

		Message msg = tokenizeLine(line);
		executeCmd(client, msg);
	}
}
