/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcompain <rcompain@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 12:00:27 by rcompain          #+#    #+#             */
/*   Updated: 2026/07/03 14:31:32 by rcompain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* ——— Constructor & Destructor ————————————————————————————————————————————— */
Server::Server()
{
	//_commandsMap["PASS"] = fonction
}

/* ——— Getters & Setters ———————————————————————————————————————————————————— */

/* ——— Functions         ———————————————————————————————————————————————————— */

void Server::handleClientData(Client &client)
{
	std::string &buf = client.getReadBuffer();
	size_t		 pos;

	while ((pos = buf.find("\r\n")) != std::string::npos) {
		std::string line = buf.substr(0, pos);
		buf.erase(0, pos + 2);

		Message msg = tokenizeLine(line);
		executeCmd(client, msg);
	}
}
