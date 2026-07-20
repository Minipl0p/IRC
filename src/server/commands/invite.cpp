
#include "../../channels/Channel.hpp"
#include "../../reply/Reply.hpp"
#include "../Server.hpp"

void Server::invite(Client &client, std::vector<std::string> &params)
{
	if (!requireRegistered(client))
		return;
	if (!requireParams(client, params, 2, "INVITE"))
		return;

	Channel *chan = requireChannel(client, params[1]);
	if (!chan)
		return;

	if (!requireMember(client, chan))
		return;

	if (chan->isInviteOnly() && !requireOperator(client, chan))
		return;

	Client *target = requireClientByNick(client, params[0]);
	if (!target)
		return;

	if (!requireTargetNotInChannel(client, *target, chan))
		return;

	chan->addInviteMember(*target);
	std::string msg = ":" + client.getNickname() + "!" + client.getUsername() +
					  "@localhost INVITE " + target->getNickname() + " :" + chan->getName() +
					  "\r\n";
	send(target->getFd(), msg.c_str(), msg.size(), 0);
	sendReply(client, RPL_INVITING, target->getNickname() + " " + chan->getName());
}
