/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchazalm <pchazalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 12:00:27 by rcompain          #+#    #+#             */
/*   Updated: 2026/07/20 18:15:31 by pchazalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* ——— Constructor & Destructor ————————————————————————————————————————————— */
Server::Server(int port, std::string password)
{
	//_commandsMap["PASS"] = fonction

	try {
		initServ(port, password);
	} catch (const std::runtime_error &error) {
		std::cout << error.what() << std::endl;
		std::exit(-1);
	}

	initCommands();
}

Server::~Server()
{
	// Destroy lst client
}

/* ——— Getters & Setters ———————————————————————————————————————————————————— */
std::vector<pollfd>						&Server::getLstFds() { return _pollfds; }
const std::map<std::string, Command>	&Server::getCommandsMap() { return _commandsMap; }
std::map<fd, Client *> 					&Server::getListeningClientsMap() { return _listeningClientsMap; }
const int			   					&Server::getServerSocket() const { return _serverSocket; }
const sockaddr_in	   					&Server::getServerAddress() const { return _serverAddress; }
const pollfd		 					&Server::getServerFd() const { return _pollfds[0]; }
const std::map<std::string, Channel *>	&Server::getLstChannels() { return _lstChannels;}


/* ——— Functions         ———————————————————————————————————————————————————— */
void Server::initServ(int port, std::string password)
{
	// AF_INET : IPv4 protocol
	// SOCK_STREAM: TCP socket
	this->_password = password;

	// Gestion du socket du serveur
	this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket < 0) {
		throw std::runtime_error("ServerSocket failed.");
	}
	fcntl(_serverSocket, F_SETFL, O_NONBLOCK);
	this->_pollfds.push_back((struct pollfd){_serverSocket, POLLIN, 0});

	// Initialisation de la structure serverAddress
	std::memset(&(_serverAddress), 0, sizeof(_serverAddress));
	this->_serverAddress.sin_family = AF_INET;
	this->_serverAddress.sin_port	= htons(port);
	std::cout << this->_serverAddress.sin_port << std::endl;
	this->_serverAddress.sin_addr.s_addr = INADDR_ANY;

	int opt = 1;
	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		close(_serverSocket);
		throw std::runtime_error("SetSockopt failed.");
	}

	if (bind(_serverSocket, (struct sockaddr *)&_serverAddress, sizeof(_serverAddress)) < 0) {
		close(_serverSocket);
		if (errno == EADDRINUSE)
			throw std::runtime_error("Port already used.");
		else
			throw std::runtime_error("Bind failed.");
	}

	if (listen(_serverSocket, 5) < 0) { // 5 est la taille de la file d'attente
		close(_serverSocket);
		throw std::runtime_error("Listen failed.");
	}
}

void Server::handleClientData(Client &client)
{
	std::string &buf = client.getReadBuffer();
	size_t		 pos;

	while ((pos = buf.find("\r\n")) != std::string::npos) {
		size_t lineLen = pos;

		std::string line = buf.substr(0, lineLen);
		buf.erase(0, pos + 2);

		Message msg = tokenizeLine(line);
		executeCmd(client, msg);
	}
}


/* ——— Channel management ——————————————————————————————————————————————— */
void Server::addChannelToLst(Channel &src){
	_lstChannels[src.getName()] = &src;
}
bool Server::findChannelToLst(std::string &name) const{
	std::map<std::string, Channel *>::const_iterator it = _lstChannels.find(name);
	if (it == _lstChannels.end())
		return false;
	return true;
}
void Server::deleteChannelToLst(Channel *src){
	_lstChannels.erase(src->getName());
	delete src;
}

/* ——— FdLst management ————————————————————————————————————————————————— */
void 	Server::deletePollfdsToLst(const fd &srcFd){
	std::vector<pollfd>::iterator pos = _pollfds.begin();
	for (; pos != _pollfds.end(); ++pos){
		if (pos->fd == srcFd)
			break;
	}
	if (pos != _pollfds.end())
		_pollfds.erase(pos);

	close(srcFd);
}

/* ——— Client management ———————————————————————————————————————————————— */
void	Server::addClientsToLst(Client &src){
	_listeningClientsMap[src.getFd()] = &src;
}
bool	Server::findClientsToLst(fd &src) const{
	std::map<fd, Client *>::const_iterator it = _listeningClientsMap.find(src);
	if (it == _listeningClientsMap.end())
		return false;
	return true;
}
void	Server::deleteClientsToLst(Client *src, std::string &msg){

	// enlever le client de chanque channel ou il est present
	const std::map<std::string, Channel *> &lst = getLstChannels();
	for (std::map<std::string, Channel *>::const_iterator it = lst.begin(); it != lst.end();){
		Channel *chan = it->second;
		++it;
		if (chan->isMember(*src)){
			if (!msg.empty())
				sendToChannel(*chan, msg);
			chan->removeMember(*src);
			if (chan->isEmptyChannel())
				deleteChannelToLst(chan);
		}
	}

	_listeningClientsMap.erase(src->getFd());
	deletePollfdsToLst(src->getFd());
	delete src;
}