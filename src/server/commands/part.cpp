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

void Server::part(Client &client, std::vector<std::string> &params)
{
	if (!requireRegistered(client))
		return;
	if (!requireParams(client, params, 1, "PART"))
		return;

	
	Channel *chan = requireChannel(client, params[0]);
	if (!chan)
		return;
	if (!requireMember(client, chan))
		return;


	std::string comment = (params.size() >= 2) ? params[1] : "";
	std::string msg =
		":" + client.getNickname() + " PART " + chan->getName() + " :" + comment;
	sendToChannel(*chan, msg);
	chan->removeMember(client);


	if (chan->isEmptyChannel()){
		deleteChannelToLst(chan);
	}
}