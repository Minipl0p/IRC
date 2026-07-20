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

#include "../Server.hpp"
#include "../../reply/Reply.hpp"
#include <new>

void Server::join(Client &client, std::vector<std::string> &params)
{
    if (!requireRegistered(client))
        return;
    if (!requireParams(client, params, 1, "JOIN"))
        return;
    if (params[0].empty() || params[0][0] != '#') {
        sendReply(client, ERR_NOSUCHCHANNEL, params[0] + " :No such channel");
        return;
    }

    std::string key = (params.size() > 1) ? params[1] : "";
    Channel       *chan;

    if (findChannelToLst(params[0])) {
        chan = requireChannel(client, params[0]);
        if (!chan)
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
    } else {
        try {
            chan = new Channel(params[0]);
        } catch (const std::bad_alloc &) {
            return;
        }
        addChannelToLst(*chan);
        chan->addMember(client);
        chan->addModerator(client);
        goto end;
    }

    chan->addMember(client);
end:
    sendToChannel(*chan,
                  ":" + client.getNickname() + "!" + client.getUsername() +
                      "@localhost JOIN :" + chan->getName());
    sendReply(client, RPL_TOPIC, chan->getName() + " :" + chan->getTopic());
    sendReply(client, RPL_NAMREPLY, "= " + chan->getName() + " :" + chan->getMembersList());
    sendReply(client, RPL_ENDOFNAMES, chan->getName() + " :End of /NAMES list");
}