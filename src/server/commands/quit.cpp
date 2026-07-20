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
	std::string comment;

	if (params.size() >= 1)
		comment = params[0];
	else
		comment = "";

	std::string msg = ":" + client.getNickname() + "!" + client.getUsername() + " QUIT :" + comment;

	for (ChanIt it = _lstChannels.begin(); it != _lstChannels.end(); ++it) {
		Channel *chan = it->second;
		if (chan->isMember(client)) {
			sendToChannel(*chan, msg);
			chan->removeMember(client);
		}
	}

	deleteClientsToLst(&client);
}
