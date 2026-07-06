
#include "../reply/Reply.hpp"
#include "../server/Server.hpp"
#include <cctype>

void Server::initCommands()
{
	_commandsMap["PASS"] = &Server::pass;
	_commandsMap["NICK"] = &Server::nick;
	_commandsMap["USER"] = &Server::user;
	// AJOUTER LES AUTRES
}

void Server::executeCmd(Client &client, Message &msg)
{
	if (msg.command.empty())
		return;

	// toupper the command because IRC protocol is not case sensitive
	std::string cmd = msg.command;
	for (size_t i = 0; i < cmd.size(); i++)
		cmd[i] = std::toupper(cmd[i]);

	std::map<std::string, Command>::iterator it = _commandsMap.find(cmd);
	if (it == _commandsMap.end()) {
		sendReply(client, ERR_UNKNOWNCOMMAND, cmd + " :Unknown command");
		return;
	}
	(this->*(it->second))(client, msg.params);
}

void Server::tryRegister(Client &client)
{
	if (client.isAuthenticated())
		return;
	if (client.isPassOk() && !client.getNickname().empty() && !client.getUsername().empty()) {
		client.setAuthenticated(true);
		sendReply(client, RPL_WELCOME, ":Welcome to the IRC network, " + client.getNickname());
	}
}
