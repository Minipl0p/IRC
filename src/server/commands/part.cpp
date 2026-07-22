/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchazalm <pchazalm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 16:07:55 by rcompain          #+#    #+#             */
/*   Updated: 2026/07/20 18:21:41 by pchazalm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../channels/Channel.hpp"
#include "../Server.hpp"

void Server::partChannel(Client &client, const std::string &chanName, const std::string &comment)
{
	Channel *chan = requireChannel(client, chanName);
	if (!chan || !requireMember(client, chan))
		return;

	sendToChannel(*chan,
				  ":" + client.getNickname() + "!" + client.getUsername() + "@localhost PART " +
					  chan->getName() + " :" + comment);
	chan->removeMember(client);

	if (chan->isEmptyChannel())
		deleteChannelToLst(chan);
}

void Server::part(Client &client, std::vector<std::string> &params)
{
	if (!requireRegistered(client) || !requireParams(client, params, 1, "PART"))
		return;

	std::vector<std::string> channels = split(params[0], ',');
	std::string				 comment  = (params.size() >= 2) ? params[1] : "";

	for (size_t i = 0; i < channels.size(); i++)
		partChannel(client, channels[i], comment);
}
