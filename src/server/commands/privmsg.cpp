
#include "../../channels/Channel.hpp"
#include "../Server.hpp"
#include <sstream>

static std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> out;
    std::stringstream         ss(s);
    std::string                 item;

    while (std::getline(ss, item, delim)) {
        out.push_back(item);
    }
    return out;
}

void Server::privmsg(Client &client, std::vector<std::string> &params)
{
    if (!requireRegistered(client))
        return;
    if (!requireRecipient(client, params))
        return;

    std::string text = (params.size() >= 2) ? params[1] : "";
    if (!requireTextToSend(client, text))
        return;

    std::vector<std::string> targets = split(params[0], ',');

    for (size_t i = 0; i < targets.size(); ++i) {
        const std::string &targetName = targets[i];

        if (!targetName.empty() && targetName[0] == '#') {
            Channel *chan = requireChannel(client, targetName);
            if (!chan)
                continue;
            if (!requireMember(client, chan))
                continue;
            std::string msg = ":" + client.getNickname() + "!" + client.getUsername() +
                              "@localhost PRIVMSG " + chan->getName() + " :" + text;
            sendToChannel(*chan, msg);

        } else {
            std::string msg = ":" + client.getNickname() + "!" + client.getUsername() +
                              "@localhost PRIVMSG " + targetName + " :" + text;
            Client       *dst = requireClientByNick(client, targetName);
            if (!dst)
                continue;
            send(dst->getFd(), msg.c_str(), msg.size(), 0);
        }
    }
}