/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaulnie <psaulnie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 11:10:46 by psaulnie          #+#    #+#             */
/*   Updated: 2023/02/15 16:06:55 by psaulnie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Bot.hpp"

Bot::Bot(SocketIO &io) : _io(io)
{
	_scoob_msg = "You are in ScoobIRC, the best IRC server !";
	_velma_msg = "Jinkies!";
	_shaggy_msg = "Zoinks Scoob!";
	_curr_time = std::time(NULL);
}

Bot::~Bot() { }

void	Bot::setMsg(std::vector<std::string> &input, User &cUser)
{
	std::string	who = input[2];
	std::string	new_msg;

	std::vector<std::string>::iterator it = input.begin();
	it++; it++; it++;

	while (it != input.end())
	{
		new_msg.append(*it);
		if (++it != input.end() && *it != "")
			new_msg.append(" ");
	}
	if (who == ":SCOOBY" || who == ":SCOOB" || who == ":SCOOBY-DOO")
		_scoob_msg = new_msg;
	else if (who == ":VELMA")
		_velma_msg = new_msg;
	else if (who == ":SHAGGY")
		_shaggy_msg = new_msg;
	else
	{
		_io.emit(":TheMysteryMachine NOTICE Usage => /msg TheMysteryMachine [SCOOBY, VELMA, SHAGGY] message ... ...\r\n", cUser.getFd());
		return ;
	}
	_io.emit(":TheMysteryMachine NOTICE " + cUser.getNick() + " " + who.substr(1, who.length() - 1) + " will now say: " + "\"" + new_msg + "\""+ "\r\n", cUser.getFd());
}

void	Bot::check(std::vector<User> &clients)
{
	if (std::difftime(std::time(NULL), _curr_time) >= 300)
	{
		sendMsg(clients);
		_curr_time = std::time(NULL);
	}
}

void	Bot::sendMsg(std::vector<User> &clients)
{
	std::vector<User>::iterator it = clients.begin();
	it++;
	for (it; it != clients.end(); it++)
	{
		if (it->getFd() != -1)
		{
			_io.emit(":SCOOBY-BOT NOTICE " + it->getNick() + " " + _scoob_msg + "\r\n", it->getFd());
			_io.emit(":VELMA NOTICE " + it->getNick() + " " + _velma_msg + "\r\n", it->getFd());
			_io.emit(":SHAGGY NOTICE " + it->getNick() + " " + _shaggy_msg + "\r\n", it->getFd());

		}
	}
}
