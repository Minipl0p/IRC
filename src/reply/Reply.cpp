
#include "../server/Server.hpp"
#include <sys/socket.h>

void Server::sendReply(Client &client, const std::string &code, const std::string &params)
{
	std::string nick;
	if (client.getNickname().empty())
		nick = '*';
	else
		nick = client.getNickname();
	std::string msg = ":ircserv " + code + " " + nick + " " + params + "\r\n";
	send(client.getFd(), msg.c_str(), msg.size(), 0);
}

void Server::sendToChannel(const Channel &chan, const std::string &str)
{
	std::string							   msg = str + "\r\n";
	const std::map<std::string, Client *> &lst = chan.getMembers();
	for (std::map<std::string, Client *>::const_iterator it = lst.begin(); it != lst.end(); it++) {
		send(it->second->getFd(), msg.c_str(), msg.size(), 0);
	}
}

void Server::sendToChannel(const Channel &chan, const std::string &str, Client &exept)
{
	std::string							   msg = str + "\r\n";
	const std::map<std::string, Client *> &lst = chan.getMembers();
	for (std::map<std::string, Client *>::const_iterator it = lst.begin(); it != lst.end(); it++) {
		if (it->second == &exept)
			continue;
		send(it->second->getFd(), msg.c_str(), msg.size(), 0);
	}
}
