
#include "00_Server.hpp"

void Server::pass(Client &client, std::vector<std::string> &params)
{
	// Already authentificated
	if (client.isAuthenticated()) {
		// QUOI RENVOYER
		return;
	}

	// No params -> error
	if (params.empty()) {
		// QUOI RENVOYER
		return;
	}

	// Check password
	if (params[0] != _password) {
		// QUOI RENVOYER
		return;
	}

	client.setPassOk(true);
}

void Server::nick(Client &client, std::vector<std::string> &params)
{
	// is password was send first
	if (!client.isPassOk()) {
		// QUOI RENVOYER
		return;
	}

	// Pseudo are necessary
	if (params.empty()) {
		// QUOI RENVOYER
		return;
	}

	std::string newNick = params[0];

	// 3. (optionnel mais propre) valider les caractères autorisés
	//    Un pseudo IRC ne peut pas contenir espaces, ',', '*', etc.
	//    Sinon → 432 ERR_ERRONEUSNICKNAME

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
	// first checks
	if (!client.isPassOk()) {
		// QUOI RENVOYER
		return;
	}
	if (client.isAuthenticated()) {
		// QUOI RENVOYER
		return;
	}

	// USER need 4 params : <username> <mode> <unused> :<realname>
	if (params.size() < 4) {
		// sendReply(client, 461, "USER :Not enough parameters");
		return;
	}

	client.setUsername(params[0]);
	client.setRealname(params[3]);

	tryRegister(client);
}
