
#include "../../reply/Reply.hpp"
#include "../Server.hpp"

void Server::pass(Client &client, std::vector<std::string> &params)
{
	if (client.isAuthenticated()) {
		sendReply(client, ERR_ALREADYREGISTRED, ":You may not reregister");
		return;
	}

	if (params.empty()) {
		sendReply(client, ERR_NEEDMOREPARAMS, "PASS :Not enough parameters");
		return;
	}

	if (params[0] != _password) {
		sendReply(client, ERR_PASSWDMISMATCH, ":Password incorrect");
		return;
	}

	client.setPassOk(true);
}
