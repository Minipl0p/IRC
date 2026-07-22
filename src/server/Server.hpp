
#pragma once

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <netinet/in.h> //socketadress
#include <poll.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#include "../channels/Channel.hpp"
#include "../clients/Client.hpp"
#include "../parsing/Message.hpp"

/* ——— Forward declarations ————————————————————————————————————————————————— */

typedef int fd;
class Server;
typedef void (Server::*Command)(Client &client, std::vector<std::string> &params);

class Server
{
  private:
	/* ——— Attributes ——————————————————————————————————————————————————————— */
	std::string						 _password;
	std::vector<pollfd>				 _pollfds;
	std::map<fd, Client *>			 _listeningClientsMap;
	std::map<std::string, Channel *> _lstChannels;
	std::map<std::string, Command>	 _commandsMap;
	int								 _serverSocket;
	sockaddr_in						 _serverAddress;

	/* ——— Setup ———————————————————————————————————————————————————————————— */
	void initCommands();
	void initServ(int port, std::string password);

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

	/* ——— Command check helpers ———————————————————————————————————————————— */
	bool	 requireRegistered(Client &client);
	bool	 requireParams(Client					&client,
						   std::vector<std::string> &params,
						   size_t					 min,
						   const std::string		&cmdName);
	Channel *requireChannel(Client &client, const std::string &name);
	Client	*requireClientByNick(Client &client, const std::string &nick);
	bool	 requireMember(Client &client, Channel *chan);
	bool	 requireOperator(Client &client, Channel *chan);
	bool	 requireTargetInChannel(Client &asker, Client &target, Channel *chan);
	bool	 requireTargetNotInChannel(Client &asker, Client &target, Channel *chan);
	bool	 requireRecipient(Client &client, std::vector<std::string> &params);
	bool	 requireTextToSend(Client &client, const std::string &text);

	/* ——— Channel management ——————————————————————————————————————————————— */
	void addChannelToLst(Channel &);
	bool findChannelToLst(const std::string &name) const;
	void deleteChannelToLst(Channel *);
	void sendToChannel(const Channel &, const std::string &);
	void sendToChannel(const Channel &, const std::string &, const Client &);
	void joinChannel(Client &client, const std::string &chanName, const std::string &key);
	void partChannel(Client &client, const std::string &chanName, const std::string &comment);

	/* ——— Replies —————————————————————————————————————————————————————————— */
	void sendReply(Client &client, const std::string &code, const std::string &params);

  public:
	/* ——— Constructor & Destructor ————————————————————————————————————————— */
	Server(int port, std::string password);
	~Server();

	/* ——— Main entry point ————————————————————————————————————————————————— */
	void handleClientData(Client &);

	/* ——— FdLst management ————————————————————————————————————————————————— */
	void deletePollfdsToLst(const fd &);

	/* ——— Life loop server ————————————————————————————————————————————————— */
	void						  acceptNewClient();
	std::vector<pollfd>::iterator handleClientEvent(std::vector<pollfd>::iterator it);

	/* ——— Client management ———————————————————————————————————————————————— */
	void	addClientsToLst(Client &);
	bool	findClientsToLst(fd &) const;
	void	deleteClientsToLst(Client *, std::string &);
	Client *isClientExistOnServer(const std::string &);

	/* ——— Getters & Setters ———————————————————————————————————————————————— */
	std::vector<pollfd>					   &getLstFds();
	const std::map<std::string, Command>   &getCommandsMap();
	std::map<fd, Client *>				   &getListeningClientsMap();
	const std::map<std::string, Channel *> &getLstChannels();
	const int							   &getServerSocket() const;
	const sockaddr_in					   &getServerAddress() const;
	const pollfd						   &getServerFd() const;
};

std::vector<std::string> split(const std::string &s, char delim);
