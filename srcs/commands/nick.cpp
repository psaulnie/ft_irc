/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaulnie <psaulnie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 13:10:55 by psaulnie          #+#    #+#             */
/*   Updated: 2023/02/15 17:13:19 by psaulnie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/Server.hpp"

void	Server::nickCmd(std::vector<std::string> &input, int fd, User &cUser)
{
	if (input[1] == "")
	{
		_rep.E431(fd, cUser.getNick());
		return ;
	}
	if (input[1].size() > 9) // TODO check unwanted character
	{
		_rep.E432(fd, cUser.getNick(), input[1]);
		return ;
	}
	std::vector<User>::iterator it = _clients.begin();
	for (it; it < _clients.end(); it++)
	{
		if (it->getNick() == input[1] || input[1] == "TheMysteryMachine")
		{
			_rep.E433(fd, cUser.getNick(), input[1]);
			return;
		}
	}
	if (cUser.getRegister())
		_rep.R001(fd, input[1]);
	cUser.setNick(input[1]); //TOCHECK: set index 1 for the moment but review the input vector construction (wich index for the nickname) (?)
}
