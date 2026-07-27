/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcompain <rcompain@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 21:46:45 by rcompain          #+#    #+#             */
/*   Updated: 2026/07/21 21:51:01 by rcompain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../reply/Reply.hpp"
#include "../Server.hpp"

void Server::ping(Client &client, std::vector<std::string> &params)
{
	if (!requireRegistered(client))
		return;

	if (!requireParams(client, params, 1, "PING"))
		return;

	std::string	name= "botte";
	std::string msg = ":" + name + "!" + name +
		"@localhost PRIVMSG " + client.getNickname() + " :" + "PONG !\r\n";
	send(client.getFd(), msg.c_str(), msg.size(), 0);
}
