
#include "../../reply/Reply.hpp"
#include "../Server.hpp"
#include <cctype>

static std::string toLower(const std::string &s)
{
	std::string result = s;
	for (size_t i = 0; i < result.size(); i++)
		result[i] = std::tolower(result[i]);
	return result;
}

static bool isValidNickname(const std::string &nick)
{
	if (nick.empty() || nick.size() > 9)
		return false;

	const std::string special = "[]\\`_^{|}";

	char c = nick[0];
	if (!std::isalpha(c) && special.find(c) == std::string::npos)
		return false;

	for (size_t i = 1; i < nick.size(); i++) {
		c = nick[i];
		if (!std::isalnum(c) && special.find(c) == std::string::npos && c != '-')
			return false;
	}

	return true;
}

void Server::nick(Client &client, std::vector<std::string> &params)
{
	if (!client.isPassOk()) {
		sendReply(client, ERR_PASSWDMISMATCH, ":Password required");
		return;
	}

	if (params.empty()) {
		sendReply(client, ERR_NONICKNAMEGIVEN, ":No nickname given");
		return;
	}

	std::string newNick = params[0];

	if (!isValidNickname(newNick)) {
		sendReply(client, ERR_ERRONEUSNICKNAME, newNick + " :Erroneous nickname");
		return;
	}

	std::map<int, Client *>::iterator it;
	for (it = _listeningClientsMap.begin(); it != _listeningClientsMap.end(); ++it) {
		if (toLower(it->second->getNickname()) == toLower(newNick)) {
			sendReply(client, ERR_NICKNAMEINUSE, newNick + " :Nickname is already in use");
			return;
		}
	}

	client.setNickname(newNick);

	tryRegister(client);
}
