/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcompain <rcompain@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 12:00:27 by rcompain          #+#    #+#             */
/*   Updated: 2026/07/06 12:28:14 by rcompain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "00_Server.hpp"

/* ——— Constructor & Destructor ————————————————————————————————————————————— */
Server::Server(){
	//_commandsMap["PASS"] = fonction

	initServ();
}



/* ——— Getters & Setters ———————————————————————————————————————————————————— */


/* ——— Methodes ————————————————————————————————————————————————————————————— */
void Server::initServ(){
	this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	//AF_INET : IPv4 protocol
	//SOCK_STREAM: TCP socket
	this->serverAddress.sin_family = AF_INET;
	this->serverAddress.sin_port = htons(8000);
	this->serverAddress.sin_addr.s_addr = INADDR_ANY;
	bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	listen(serverSocket, 5); // 5 est la taille de la file d'attente
}