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

#include "00_Server.hpp"

/* ——— Constructor & Destructor ————————————————————————————————————————————— */
Server::Server()
{
	//_commandsMap["PASS"] = fonction
}

/* ——— Getters & Setters ———————————————————————————————————————————————————— */

/* ——— Functions         ———————————————————————————————————————————————————— */

void Server::executeCmd(Client &client, Message &msg)
{
	if (msg.command.empty())
		return;

	// toupper the command because IRC protocole is not case sensitive
	std::string cmd = msg.command;
	for (size_t i = 0; i < cmd.size(); i++)
		cmd[i] = std::toupper(cmd[i]);

	// 3. Chercher la commande dans la map
	std::map<std::string, Command>::iterator it = _commandsMap.find(cmd);

	if (it == _commandsMap.end()) {
		// CHECK QUOI FAIT LE PROTOCOLE IRC EN CAS DE COMMANDE INVALIDE
		return;
	}
	(this->*(it->second))(client, msg.params);
}

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

