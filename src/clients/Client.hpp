/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_Client.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcompain <rcompain@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 12:00:12 by rcompain          #+#    #+#             */
/*   Updated: 2026/07/03 13:10:42 by rcompain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class Client
{
  private:
	int			_fd;
	std::string _nickname;
	std::string _username;
	std::string _fullname;
	bool		_authenticated;
	bool		_passOk;
	std::string _readBuffer;
	bool		_disconnected;

  public:
	Client(int fd);
	~Client();

	// Getters -----------------------------------------------------------------
	int				   getFd() const;
	const std::string &getNickname() const;
	const std::string &getUsername() const;
	const std::string &getFullname() const;
	bool			   isAuthenticated() const;
	bool			   isPassOk() const;
	std::string		  &getReadBuffer();
	bool			   isDisconnected() const;

	// Setter ------------------------------------------------------------------
	void setNickname(const std::string &);
	void setUsername(const std::string &);
	void setFullname(const std::string &);
	void setAuthenticated(bool);
	void setPassOk(bool);
	void setDisconnected(bool v);
};
