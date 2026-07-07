
#include "../../channels/Channel.hpp"
#include "../../reply/Reply.hpp"
#include "../Server.hpp"

bool Server::requireRegistered(Client &client)
{
	if (!client.isAuthenticated()) {
		sendReply(client, ERR_NOTREGISTERED, ":You have not registered");
		return false;
	}
	return true;
}

bool Server::requireParams(Client					&client,
						   std::vector<std::string> &params,
						   size_t					 min,
						   const std::string		&cmdName)
{
	if (params.size() < min) {
		sendReply(client, ERR_NEEDMOREPARAMS, cmdName + " :Not enough parameters");
		return false;
	}
	return true;
}

Channel *Server::requireChannel(Client &client, const std::string &name)
{
	std::map<std::string, Channel *>::iterator it = _lstChannels.find(name);
	if (it == _lstChannels.end()) {
		sendReply(client, ERR_NOSUCHCHANNEL, name + " :No such channel");
		return NULL;
	}
	return it->second;
}

Client *Server::requireClientByNick(Client &client, const std::string &nick)
{
	Client *target = isClientExistOnServer(nick);
	if (!target) {
		sendReply(client, ERR_NOSUCHNICK, nick + " :No such nick/channel");
		return NULL;
	}
	return target;
}

bool Server::requireMember(Client &client, Channel *chan)
{
	if (!chan->isMembre(client)) {
		sendReply(client, ERR_NOTONCHANNEL, chan->getName() + " :You're not on that channel");
		return false;
	}
	return true;
}

bool Server::requireOperator(Client &client, Channel *chan)
{
	if (!chan->isModerator(client)) {
		sendReply(client, ERR_CHANOPRIVSNEEDED, chan->getName() + " :You're not channel operator");
		return false;
	}
	return true;
}

bool Server::requireTargetInChannel(Client &asker, Client &target, Channel *chan)
{
	if (!chan->isMembre(target)) {
		sendReply(asker,
				  ERR_USERNOTINCHANNEL,
				  target.getNickname() + " " + chan->getName() + " :They aren't on that channel");
		return false;
	}
	return true;
}

bool Server::requireTargetNotInChannel(Client &asker, Client &target, Channel *chan)
{
	if (chan->isMembre(target)) {
		sendReply(asker,
				  ERR_USERONCHANNEL,
				  target.getNickname() + " " + chan->getName() + " :is already on channel");
		return false;
	}
	return true;
}

bool Server::requireRecipient(Client &client, std::vector<std::string> &params)
{
	if (params.empty()) {
		sendReply(client, ERR_NORECIPIENT, ":No recipient given (PRIVMSG)");
		return false;
	}
	return true;
}

bool Server::requireTextToSend(Client &client, const std::string &text)
{
	if (text.empty()) {
		sendReply(client, ERR_NOTEXTTOSEND, ":No text to send");
		return false;
	}
	return true;
}

Client *Server::isClientExistOnServer(std::string str){
	(void)str;
	return NULL;
}
