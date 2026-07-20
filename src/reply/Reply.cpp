
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
	std::string msg = str + "\r\n";
	std::map<std::string, Client *> members = chan.getMembers();
	for (CliIt it = members.begin(); it != members.end(); it++) {
		send(it->second->getFd(), msg.c_str(), msg.size(), 0);
	}
}
