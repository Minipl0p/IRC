/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchazalm <pchazalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 14:00:17 by rcompain          #+#    #+#             */
/*   Updated: 2026/07/20 19:04:02 by pchazalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../reply/Reply.hpp"
#include "../Server.hpp"
#include <new>

void Server::joinChannel(Client &client, const std::string &chanName, const std::string &key)
{
	if (chanName.empty() || chanName[0] != '#') {
		sendReply(client, ERR_NOSUCHCHANNEL, chanName + " :No such channel");
		return;
	}

	Channel *chan;

	if (findChannelToLst(chanName)) {
		chan = requireChannel(client, chanName);
		if (!chan)
			return;
		if (chan->isMember(client))
			return;
		if (chan->isInviteOnly() && !chan->isInvited(client)) {
			sendReply(client, ERR_INVITEONLYCHAN, chan->getName() + " :Cannot join channel (+i)");
			return;
		}
		if (chan->hasLimit() && chan->memberCount() >= chan->getLimit()) {
			sendReply(client, ERR_CHANNELISFULL, chan->getName() + " :Cannot join channel (+l)");
			return;
		}
		if (chan->hasKey() && !chan->keyIsValid(key)) {
			sendReply(client, ERR_BADCHANNELKEY, chan->getName() + " :Cannot join channel (+k)");
			return;
		}
		chan->addMember(client);
		chan->removeInvite(client);

	} else {
		try {
			chan = new Channel(chanName);
		} catch (const std::bad_alloc &) {
			return;
		}
		addChannelToLst(*chan);
		chan->addMember(client);
		chan->removeInvite(client);
		chan->addModerator(client);
	}

	sendToChannel(*chan,
				  ":" + client.getNickname() + "!" + client.getUsername() +
					  "@localhost JOIN :" + chan->getName());
	if (chan->getTopic().empty())
		sendReply(client, RPL_NOTOPIC, chan->getName() + " :No topic is set");
	else {
		sendReply(client, RPL_TOPIC, chan->getName() + " :" + chan->getTopic());
		std::string setter = chan->getTopicSetter().empty() ? "unknown" : chan->getTopicSetter();
		sendReply(client,
				  RPL_TOPICWHOTIME,
				  chan->getName() + " " + setter + " " + chan->getTopicTimestamp());
	}
	sendReply(client, RPL_NAMREPLY, "= " + chan->getName() + " :" + chan->getMembersList());
	sendReply(client, RPL_ENDOFNAMES, chan->getName() + " :End of /NAMES list");
}

void Server::join(Client &client, std::vector<std::string> &params)
{
	if (!requireRegistered(client))
		return;
	if (!requireParams(client, params, 1, "JOIN"))
		return;

	std::vector<std::string> channels = split(params[0], ',');
	std::vector<std::string> keys	  = std::vector<std::string>();
	if (params.size() > 1)
		keys = split(params[1], ',');

	for (size_t i = 0; i < channels.size(); i++) {
		std::string key = (i < keys.size()) ? keys[i] : "";
		joinChannel(client, channels[i], key);
	}
}
