
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
	void privmsg(Client &client, std::vector<std::string> &params);
	void part(Client &client, std::vector<std::string> &params);
	void quit(Client &client, std::vector<std::string> &params);
	void ping(Client &client, std::vector<std::string> &params);
	void kick(Client &client, std::vector<std::string> &params);
	void invite(Client &client, std::vector<std::string> &params);
	void topic(Client &client, std::vector<std::string> &params);
	void mode(Client &client, std::vector<std::string> &params);

	/* ——— Channel management ——————————————————————————————————————————————— */
	void addChannelToLst(Channel &);
	bool findChannelToLst(Channel &) const;
	void deleteChannelToLst(Channel &);

	/* ——— Replies —————————————————————————————————————————————————————————— */
	void sendReply(Client &client, const std::string &code, const std::string &params);

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
