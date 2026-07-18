/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcompain <rcompain@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 16:27:20 by rcompain          #+#    #+#             */
/*   Updated: 2026/07/18 17:43:24 by rcompain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../channels/Channel.hpp"
#include "../Server.hpp"



void Server::quit(Client &client, std::vector<std::string> &params){
	
	std::string comment = (params.size() == 1) ? params[0] : "";
	std::string msg = ":" + client.getNickname() + " QUIT " + ":" + comment;
	deleteClientsToLst(&client, msg);
}