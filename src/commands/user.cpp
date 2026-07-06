
#include "../server/Server.hpp"

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
