
#include "../../channels/Channel.hpp"
#include "../../reply/Reply.hpp"
#include "../Server.hpp"
#include <sstream>

void Server::mode(Client &client, std::vector<std::string> &params)
{
	if (!requireRegistered(client))
		return;
	if (!requireParams(client, params, 1, "MODE"))
		return;
	if (params[0].empty() || params[0][0] != '#')
		return;

	Channel *chan = requireChannel(client, params[0]);
	if (!chan)
		return;

	if (params.size() == 1) {
		std::string modes = "+";
		std::string modeParams;

		if (chan->isInviteOnly())
			modes += "i";
		if (chan->isTopicLocked())
			modes += "t";
		if (chan->hasKey()) {
			modes += "k";
			modeParams += " " + chan->getKey();
		}
		if (chan->hasLimit()) {
			modes += "l";
			std::stringstream ss;
			ss << chan->getLimit();
			modeParams += " " + ss.str();
		}
		sendReply(client, RPL_CHANNELMODEIS, chan->getName() + " " + modes + modeParams);
		return;
	}

	if (!requireOperator(client, chan))
		return;

	const std::string modeStr = params[1];
	if (modeStr.empty())
		return;

	bool					 adding	  = true;
	size_t					 argIndex = 2;
	std::string				 appliedModes;
	std::vector<std::string> appliedArgs;

	for (size_t i = 0; i < modeStr.size(); ++i) {
		char m = modeStr[i];

		if (m == '+') {
			adding = true;
			appliedModes += m;
			continue;
		}
		if (m == '-') {
			adding = false;
			appliedModes += m;
			continue;
		}

		if (m == 'i') {
			chan->setInviteOnly(adding);
			appliedModes += 'i';
		} else if (m == 't') {
			chan->setTopicLocked(adding);
			appliedModes += 't';
		} else if (m == 'k') {
			if (adding) {
				if (argIndex >= params.size())
					break;
				chan->setKey(params[argIndex]);
				appliedModes += 'k';
				appliedArgs.push_back(params[argIndex]);
				++argIndex;
			} else {
				chan->removeKey();
				appliedModes += 'k';
			}
		} else if (m == 'l') {
			if (adding) {
				if (argIndex >= params.size())
					break;
				const std::string &v	   = params[argIndex];
				bool			   numeric = !v.empty();
				for (size_t j = 0; j < v.size(); ++j) {
					if (!std::isdigit(v[j])) {
						numeric = false;
						break;
					}
				}
				if (!numeric)
					break;
				chan->setLimit(std::atoi(v.c_str()));
				appliedModes += 'l';
				appliedArgs.push_back(v);
				++argIndex;
			} else {
				chan->setLimit(0);
				appliedModes += 'l';
			}
		} else if (m == 'o') {
			if (argIndex >= params.size())
				break;
			Client *target = requireClientByNick(client, params[argIndex]);
			if (!target) {
				++argIndex;
				continue;
			}
			if (!chan->isMember(*target)) {
				++argIndex;
				continue;
			}
			if (adding)
				chan->addModerator(*target);
			else
				chan->removeModerator(*target);
			appliedModes += 'o';
			appliedArgs.push_back(params[argIndex]);
			++argIndex;
		}
	}

	if (appliedModes.empty() || appliedModes == "+" || appliedModes == "-")
		return;

	std::string msg = ":" + client.getNickname() + " MODE " + chan->getName() + " " + appliedModes;
	for (size_t i = 0; i < appliedArgs.size(); ++i)
		msg += " " + appliedArgs[i];

	sendToChannel(*chan, msg);
}
