/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_Server.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcompain <rcompain@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 12:00:12 by rcompain          #+#    #+#             */
/*   Updated: 2026/07/03 14:47:10 by rcompain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include <vector>
#include <poll.h>
#include <iostream>

typedef Client;
typedef Channel;
typedef void (Server::*Command)(Client &client, std::vector<std::string> &params);
typedef int fd;

class Server
{
	public:
	/* ——— Constructor & Destructor ————————————————————————————————————————— */
		Server();
		~Server(){};
	
	/* ——— Getters & Setters ———————————————————————————————————————————————— */
		std::vector<pollfd>& getLstFds() { return _pollfds;}
		const std::map<std::string, Command>& getCommandsMap() { return _commandsMap;}
		std::map<fd, Client&>& getListeningClientsMap() { return _listeningClientsMap;}
	
	/* ——— Methodes ————————————————————————————————————————————————————————— */
		// Clients
		void addClientsToLst(Client &);
		bool findClientsToLst(Client &)const;
		void deleteClientsToLst(Client &);

		// CommandsMap
		void exectuteCmd(const std::string& srcCmd)const;
		
		//ListeningClientsMap

		//LstChannels
		void addChannelToLst(Channel &);
		bool findChannelToLst(Channel &)const;
		void deleteChannelToLst(Channel &);

	private:
		std::vector<pollfd> _pollfds;
			/* struct pollfd {
               int   fd;         file descriptor
               short events;     requested events
               short revents;    returned events 
           };  */
		const std::map<std::string, Command> _commandsMap; // Liste de commande
		// Proto: void name (Client &client, std::vector<std::string> &params)
		// CommandsMap
		void pass(Client &client, std::vector<std::string> &params);
		void nick(Client &client, std::vector<std::string> &params);
		void join(Client &client, std::vector<std::string> &params);
		// [...]
		
		std::map<fd, Client&> _listeningClientsMap;

		std::map<std::string, Channel*> _lstChannels;
		
};