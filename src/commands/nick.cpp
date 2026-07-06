
#include "../server/Server.hpp"

static bool isValidNickname(const std::string &nick)
{
	if (nick.empty() || nick.size() > 9)
		return false;

	const std::string special = "[]\\`_^{|}-";

	char c = nick[0];
	if (!std::isalpha(c) && special.find(c) == std::string::npos)
		return false;

	for (size_t i = 1; i < nick.size(); i++) {
		c = nick[i];
		if (!std::isalnum(c) && special.find(c) == std::string::npos)
			return false;
	}

	return true;
}

void Server::nick(Client &client, std::vector<std::string> &params)
{
	if (!client.isPassOk()) {
		// QUOI RENVOYER
		return;
	}

	if (params.empty()) {
		// QUOI RENVOYER
		return;
	}

	std::string newNick = params[0];

	if (!isValidNickname(newNick)) {
		// QUOI RENVOYER
		return;
	}

	// Is username already used
	std::map<int, Client *>::iterator it;
	for (it = _listeningClientsMap.begin(); it != _listeningClientsMap.end(); ++it) {
		if (it->second->getNickname() == newNick) {
			// QUOI RENVOYER
			return;
		}
	}

	client.setNickname(newNick);

	tryRegister(client);
}
