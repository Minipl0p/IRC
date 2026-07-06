
#include "00_Server.hpp"

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

void Server::pass(Client &client, std::vector<std::string> &params)
{
	if (client.isAuthenticated()) {
		// QUOI RENVOYER
		return;
	}

	if (params.empty()) {
		// QUOI RENVOYER
		return;
	}

	if (params[0] != _password) {
		// QUOI RENVOYER
		return;
	}

	client.setPassOk(true);
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

void Server::user(Client &client, std::vector<std::string> &params)
{
	if (!client.isPassOk()) {
		// QUOI RENVOYER
		return;
	}
	if (client.isAuthenticated()) {
		// QUOI RENVOYER
		return;
	}

	// USER need 4 params but only 2 are realisticly used : <username> <mode> <unused> :<realname>
	if (params.size() < 4) {
		// QUOI RENVOYER
		return;
	}

	client.setUsername(params[0]);
	client.setFullname(params[3]);

	tryRegister(client);
}

void Server::tryRegister(Client &client)
{
	if (client.isAuthenticated())
		return;
	if (client.isPassOk() && !client.getNickname().empty() && !client.getUsername().empty()) {
		client.setAuthenticated(true);
		// QUOI RENVOYER
	}
}
