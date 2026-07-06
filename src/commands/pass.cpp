
#include "../server/Server.hpp"

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
