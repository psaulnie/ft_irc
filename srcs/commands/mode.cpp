/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaulnie <psaulnie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 13:47:31 by dbouron           #+#    #+#             */
/*   Updated: 2023/02/17 12:33:25 by psaulnie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/Server.hpp"

void	Server::notAMode(std::string const &which, std::string const &input, User &cUser)
{
	if (which == "channel")
	{
		for (int i = 1; input[i]; i++)
		{
			if (input[i] != 'k' || input[i] != 'l' || input[i] != 'm' 
					|| input[i] != 'n' || input[i] != 'o' || input[i] != 'p'
					|| input[i] != 'r' || input[i] != 's' || input[i] != 't'
					|| input[i] != 'v')
				_rep.E472(cUser.getFd(), cUser.getNick(), input[i]);
		}
	}
	else if (which == "user")
	{
		for (int i = 1; input[i]; i++)
		{
			if (input[i] != 'a' || input[i] != 'c' || input[i] != 'g' 
					|| input[i] != 'h' || input[i] != 'i' || input[i] != 'o'
					|| input[i] != 's' || input[i] != 'w')
				_rep.E472(cUser.getFd(), cUser.getNick(), input[i]);
		}		
	}
}
/**
 * Change mode for a channel:	/MODE <channelName> <+|-> <mode> [parametres]
 * Change mode for a user:		/MODE <nickname> <+|-> <mode>
 * @param input
 * @param fd
 * @param cUser
 */
void Server::modeCmd(std::vector<std::string> &input, int fd, User &cUser)
{
	if (input[1].empty() || input[2].empty() || input[3].empty())
	{
		_rep.E461(fd, cUser.getNick(), input[0]); // ERR_NEEDMOREPARAMS
		return;
	}
	if (input[1][0] == '#') // mode for a channel
	{
		//	If <modestring> is not given, the RPL_CHANNELMODEIS (324) numeric is returned.
		std::vector<Channel>::iterator	it;
		for (it = _channels.begin(); it != _channels.end(); it++)
			if (it->getName() == input[1])
				break ;
		if (it == _channels.end())
		{
			_rep.E403(fd, cUser.getNick(), input[1]); // TOCHECK if enough + if need to substr the '#' from input[1]
			return ;
		}
		if ((input[2][0] != '+' || input[2][0] != '-') || input[2].length() < 2) // TOCHECK
		{
			_rep.R324(fd, cUser.getNick(), input[1], input[2], input[3]);
			return ;
		}
		notAMode("channel", input[2], cUser);
		//	If <modestring> is given,
		//	the user sending the command MUST have appropriate channel privileges on the target channel to change the modes given.
		//	If a user does not have appropriate privileges to change modes on the target channel,
		//	the server MUST NOT process the message, and ERR_CHANOPRIVSNEEDED (482) numeric is returned.
		
		//	If the user has permission to change modes on the target,
		//	the supplied modes will be applied based on the type of the mode (see below).
		//	For type A, B, and C modes, arguments will be sequentially obtained from <mode arguments>.
		//	If a type B or C mode does not have a parameter when being set, the server MUST ignore that mode.
		//	If a type A mode has been sent without an argument, the contents of the list MUST be sent to the user,
		//	unless it contains sensitive information the user is not allowed to access.
		//	When the server is done processing the modes,
		//	a MODE command is sent to all members of the channel containing the mode changes.
		//	Servers MAY choose to hide sensitive information when sending the mode changes.
	}
	else // mode for a user
	{
		//	If <target> is a nickname that does not exist on the network,
		//	the ERR_NOSUCHNICK (401) numeric is returned.
		std::vector<User>::iterator user = _clients.begin();
		for (user; user < _clients.end(); user++)
		{
			if (user->getNick() == input[1])
			{
				//	If <target> is a different nick than the user who sent the command,
				//	the ERR_USERSDONTMATCH (502) numeric is returned.
				if (cUser.getNick() != user->getNick())
				{
					_rep.E502(fd, cUser.getNick());
					return;
				}

			}
		}
		_rep.E401(fd, cUser.getNick(), input[1]);
		//	If <modestring> is not given,
		//	the RPL_UMODEIS (221) numeric is sent back containing the current modes of the target user.
		//	If <modestring> is given, the supplied modes will be applied,
		//	and a MODE message will be sent to the user containing the changed modes.
		//	If one or more modes sent are not implemented on the server,
		//	the server MUST apply the modes that are implemented,
		//	and then send the ERR_UMODEUNKNOWNFLAG (501) in reply along with the MODE message.
	}
}