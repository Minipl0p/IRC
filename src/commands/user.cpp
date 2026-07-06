
#include "../reply/Reply.hpp"
#include "../server/Server.hpp"

void Server::user(Client &client, std::vector<std::string> &params)
{
	if (!client.isPassOk()) {
		sendReply(client, ERR_PASSWDMISMATCH, ":Password required");
		return;
	}
	if (client.isAuthenticated()) {
		sendReply(client, ERR_ALREADYREGISTRED, ":You may not reregister");
		return;
	}

	// USER need 4 params but only 2 are realisticly used : <username> <mode> <unused> :<realname>
	if (params.size() < 4) {
		sendReply(client, ERR_NEEDMOREPARAMS, "USER :Not enough parameters");
		return;
	}

	client.setUsername(params[0]);
	client.setFullname(params[3]);

	tryRegister(client);
}
