/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcompain <rcompain@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 20:38:13 by rcompain          #+#    #+#             */
/*   Updated: 2026/07/21 21:06:54 by rcompain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../reply/Reply.hpp"
#include "../Server.hpp"

void Server::topic(Client &client, std::vector<std::string> &params)
{
	if (!requireRegistered(client))
		return;

	if (!requireParams(client, params, 1, "TOPIC"))
		return;

	Channel *chan = requireChannel(client, params[0]);
	if (!chan)
		return;
	if (!requireMember(client, chan))
		return;

	
	if (params.size() == 1){ // cas ou juste consultation du topic
		if (chan->getTopic().empty())
			sendReply(client, RPL_NOTOPIC, chan->getName() + " :No topic is set");
		else{
			sendReply(client, RPL_TOPIC, chan->getName() + " :" + chan->getTopic());
		}
	}
	else{ // cas ou modification du topic
		if (chan->isTopicLocked() && !requireOperator(client, chan))
			return;
		chan->setTopic(params[1]);

		std::string msg = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost TOPIC " +
					  chan->getName() + " :" + params[1] + "\r\n";
		sendToChannel(*chan, msg);
	}
}