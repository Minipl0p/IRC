/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcompain <rcompain@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 14:00:17 by rcompain          #+#    #+#             */
/*   Updated: 2026/07/15 15:21:54 by rcompain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../reply/Reply.hpp"
#include "../Server.hpp"
#include <new>

void Server::join(Client &client, std::vector<std::string> &params)
{
	if (!requireRegistered(client))
		return;
	if (!requireParams(client, params, 1, "JOIN"))
		return;

	Channel *chan;

	if (findChannelToLst(params[0])) {
		chan = requireChannel(client, params[0]);
		if (!chan)
			return;
	} else {
		try {
			chan = new Channel(params[0]);
		} catch (const std::bad_alloc &exception) {
			return;
		}
		addChannelToLst(*chan);
		chan->addModerator(client);
	}

	std::string key = (params.size() > 1) ? params[1] : "";

	// Joindre un channel deja existant
	if (chan->isInviteOnly() && !chan->isInvited(client)) {
		sendReply(client, ERR_INVITEONLYCHAN, chan->getName() + " :Cannot join channel");
		return;
	}
	if (chan->hasLimit() && chan->membreCount() >= chan->getLimit()) {
		sendReply(client, ERR_CHANNELISFULL, chan->getName() + " :Cannot join channel");
		return;
	}
	if ((key.empty() && chan->hasKey()) || !chan->keyIsValid(key)) {
		sendReply(client, ERR_PASSWDMISMATCH, " :Password incorrect");
		return;
	}

	chan->addMember(client);
	sendToChannel(*chan,
				  ":" + client.getNickname() + "!" + client.getUsername() +
					  "@localhost JOIN :" + chan->getName());
	sendReply(client, RPL_TOPIC, chan->getName() + " :" + chan->getTopic());
	sendReply(client, RPL_NAMREPLY, "= " + chan->getName() + " :" + chan->getMembersList());
	sendReply(client, RPL_ENDOFNAMES, chan->getName() + " :End of /NAMES list");
}
