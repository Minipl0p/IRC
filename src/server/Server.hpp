
#pragma once

#include <iostream>
#include <map>
#include <poll.h>
#include <string>
#include <vector>

#include "../clients/Client.hpp"
#include "../parsing/Message.hpp"

/* ——— Forward declarations ————————————————————————————————————————————————— */
class Channel;

typedef int fd;

class Server
{
	typedef void (Server::*Command)(Client &client, std::vector<std::string> &params);

  private:
	/* ——— Attributes ——————————————————————————————————————————————————————— */
	std::string						 _password;
	std::vector<pollfd>				 _pollfds;
	std::map<fd, Client *>			 _listeningClientsMap;
	std::map<std::string, Channel *> _lstChannels;
	std::map<std::string, Command>	 _commandsMap;

	/* ——— Setup ———————————————————————————————————————————————————————————— */
	void initCommands();

	/* ——— Command routing —————————————————————————————————————————————————— */
	void executeCmd(Client &client, Message &msg);
	void tryRegister(Client &client);

	/* ——— IRC commands ————————————————————————————————————————————————————— */
	void pass(Client &client, std::vector<std::string> &params);
	void nick(Client &client, std::vector<std::string> &params);
	void user(Client &client, std::vector<std::string> &params);
	void join(Client &client, std::vector<std::string> &params);

	/* ——— Channel management ——————————————————————————————————————————————— */
	void addChannelToLst(Channel &);
	bool findChannelToLst(Channel &) const;
	void deleteChannelToLst(Channel &);

  public:
	/* ——— Constructor & Destructor ————————————————————————————————————————— */
	Server();
	~Server();

	/* ——— Main entry point ————————————————————————————————————————————————— */
	void handleClientData(Client &);

	/* ——— Client management ———————————————————————————————————————————————— */
	void addClientsToLst(Client &);
	bool findClientsToLst(Client &) const;
	void deleteClientsToLst(Client &);

	/* ——— Getters & Setters ———————————————————————————————————————————————— */
	std::vector<pollfd>					 &getLstFds();
	const std::map<std::string, Command> &getCommandsMap();
	std::map<fd, Client *>				 &getListeningClientsMap();
};
