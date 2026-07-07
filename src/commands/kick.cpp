
#include "../channels/Channel.hpp"
#include "../server/Server.hpp"

void Server::kick(Client &client, std::vector<std::string> &params)
{
	if (!requireRegistered(client))
		return;
	if (!requireParams(client, params, 2, "KICK"))
		return;
	Channel *chan = requireChannel(client, params[0]);
	if (!chan)
		return;
	if (!requireMember(client, chan))
		return;
	if (!requireOperator(client, chan))
		return;
	Client *target = requireClientByNick(client, params[1]);
	if (!target)
		return;
	if (!requireTargetInChannel(client, *target, chan))
		return;

	std::string comment = (params.size() >= 3) ? params[2] : client.getNickname();

	std::string msg =
		":" + client.getNickname() + " KICK " + chan->getName() + " " + params[1] + " :" + comment;
	sendToChannel(*chan, msg);

	chan->removeMembre(*target);
}
