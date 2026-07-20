/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcompain <rcompain@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 16:27:20 by rcompain          #+#    #+#             */
/*   Updated: 2026/07/18 17:43:24 by rcompain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../channels/Channel.hpp"
#include "../Server.hpp"

void Server::quit(Client &client, std::vector<std::string> &params)
{
	std::string comment = params.size() >= 1 ? params[0] : "Leaving";

	std::string msg = ":" + client.getNickname() + "!" + client.getUsername() +
					  "@localhost QUIT :" + comment + "\r\n";

	std::vector<Channel *> toDelete;

	for (ChanIt it = _lstChannels.begin(); it != _lstChannels.end(); ++it) {
		Channel *chan = it->second;
		if (!chan->isMember(client))
			continue;

		sendToChannel(*chan, msg, client);
		chan->removeMember(client);

		if (chan->isEmptyChannel()) {
			toDelete.push_back(chan);
			continue;
		}

		if (!chan->hasOperator()) {
			Client *newOp = chan->getMembers().begin()->second;
			chan->addModerator(*newOp);
			std::string modeMsg =
				":server MODE " + chan->getName() + " +o " + newOp->getNickname() + "\r\n";
			sendToChannel(*chan, modeMsg);
		}
	}

	for (size_t i = 0; i < toDelete.size(); ++i)
		deleteChannelToLst(toDelete[i]);

	deleteClientsToLst(&client);
}
