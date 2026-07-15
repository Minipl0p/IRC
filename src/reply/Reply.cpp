
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

void Server::sendToChannel(const Channel &chan, const std::string &str) {
	const std::map<std::string, Client *> &lst = chan.getMembres();
	for (std::map<std::string, Client *>::const_iterator it = lst.begin(); it != lst.end(); it++){
		send(it->second->getFd(), str.c_str(), str.size(), 0);
	}
}
