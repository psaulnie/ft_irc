/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaulnie <psaulnie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 12:02:22 by psaulnie          #+#    #+#             */
/*   Updated: 2023/03/13 11:34:21 by psaulnie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/Server.hpp"

void    Server::usrJoinChan(User &cUser, Channel &chan)
{
	cUser.incrChanConnected();
	chan.addUser(cUser);
	chan.incrUsrCon();
	std::string userNick;
	userNick.append(cUser.getNick());
	std::vector<User> chanUsers = chan.getUsers();
	for (std::vector<User>::iterator itChanUser = chanUsers.begin(); itChanUser != chanUsers.end(); itChanUser++)
			emit(":" + userNick + " JOIN " + chan.getName(),itChanUser->getFd());
}

/**
 * Create a channel or join the channel
 * @param input
 * @param cUser
 */
void	Server::joinCmd(std::vector<std::string> &input, User &cUser)
{
	std::vector<std::string>::iterator it = input.begin();
    it++;
    if (it >= input.end())
    {
        _rep.E461(cUser.getFd(), cUser.getNick(), "JOIN");
        return;
    }
    if (input[1][0] != '#')
    {
        std::vector<User>::iterator it = _clients.begin();
        for (; it != _clients.end(); it++)
        {
            if (it->getNick() == input[1])
                break ;
        }
        if (it == _clients.end())
        {
            _rep.E401(cUser.getFd(), cUser.getNick(), input[1]);
            return ;
        }
    }
    std::vector<Channel>::iterator itChannel = _channels.begin();
    std::string str = *it;
    std::string tmp;
    std::vector<std::string> listChan;
    std::vector<std::string> listKey;
    bool isPw = false;
    for (size_t i = 0; i < str.length(); i++) {
        char c = str[i];
        if (c == ',') {
            listChan.push_back(tmp);
            tmp.clear();
        }
        else
            tmp.push_back(c);
    }
    listChan.push_back(tmp);
    tmp.clear();
    it++;
    if (it != input.end()) {
        str.clear();
        str = *it;
        for (size_t i = 0; i < str.length(); i++) {
            char c = str[i];
            if (c == ',') {
                listKey.push_back(tmp);
                tmp.clear();
            }
            else
                tmp.push_back(c);
        }
        listKey.push_back(tmp);
        tmp.clear();
        isPw = true;
    }
    str.clear();
    std::vector<std::string>::iterator itKey = listKey.begin();
    for (std::vector<std::string>::iterator itLst = listChan.begin(); itLst < listChan.end(); itLst++) {
        str = *itLst;
        if(str[0] != '#'){
			str.clear();
			continue;
		}
        str.clear();
        for (; itChannel < _channels.end(); itChannel++){
			if (itChannel->getName() == *itLst)
				break;
		}
        if (itChannel == _channels.end()) { /*Channel does not exist*/
            if (cUser.getChanConnected() > MAX_CHAN)
            {
                _rep.E405(cUser.getFd(), cUser.getNick(),*itLst);
				continue;
            }
            Channel newChan = Channel(*itLst, cUser);
            if (isPw && itKey != listKey.end()) {
               newChan.setMode('k', true);
               newChan.setPw(*itKey);
               itKey++;
           }
			_channels.push_back(newChan);
			for(itChannel = _channels.begin(); itChannel < _channels.end(); itChannel++)
				if (itChannel->getName() == *itLst)
					break;
			if (itChannel == _channels.end()) /*Channel not created*/
				continue;
			itChannel->addOpUser(cUser);
			cUser.addOpChannel(*itChannel);
			cUser.incrChanConnected();
			std::vector<User> users = itChannel->getUsers();
			for (std::vector<User>::iterator itU = users.begin(); itU != users.end(); itU++)
				if (cUser.getFd() != -1)
					_rep.R353(cUser.getFd(), cUser.getNick(), itChannel->getName(), itU->getNick(),itChannel->getChanPrefix(), itChannel->getUserPrefix(*itU));
			_rep.R366(cUser.getFd(), cUser.getNick(), itChannel->getName());
			std::string userNick;
			userNick.append(cUser.getNick());
			emit(":" + userNick + " JOIN " + itChannel->getName(),cUser.getFd());
            users.clear();
		}
        else if (!itChannel->isUser(cUser)) { /* Channel does exist */
            if (itChannel->isMode('i')) { /* Channel in Invite mode */
                if (!cUser.isInviteChan(*itChannel)) {
                    _rep.E473(cUser.getFd(), cUser.getNick(), *itLst);
					continue;
                }
				cUser.removeInviteChan(*itChannel);
            }
            if (itChannel->isMode('l')) { /* Channel with a limit of users */
				if (itChannel->getUsrCon() + 1 > itChannel->getUsrNbMax()) {
					_rep.E471(cUser.getFd(), cUser.getNick(), *itLst);
					continue;
				}
			}
            if (itChannel->isBanned(cUser)) {
                _rep.E474(cUser.getFd(), cUser.getNick(), *itLst);
				continue;
            }
            if (cUser.getChanConnected() > MAX_CHAN)
            {
                _rep.E405(cUser.getFd(), cUser.getNick(),*itLst);
				continue;
            }
            else if (itChannel->isMode('k')) { /* Channel with a key */
                if (isPw && itKey != listKey.end()) {
                    if (*itKey != itChannel->getPw()) {
                        _rep.E475(cUser.getFd(), cUser.getNick(), itChannel->getName());
						continue;
                    }
                }
                else {
                    _rep.E475(cUser.getFd(), cUser.getNick(), itChannel->getName());
					continue;
                }
            }
            if (itChannel->getIsTopic())
                _rep.R332(cUser.getFd(), cUser.getNick(), itChannel->getName(), itChannel->getSubject());
			for (itChannel = _channels.begin(); itChannel < _channels.end(); itChannel++)
				if (itChannel->getName() == *itLst)
					break;
			if (itChannel == _channels.end()){
				continue;
			}
			usrJoinChan(cUser, *itChannel);
			std::vector<User> users = itChannel->getUsers();
			for (std::vector<User>::iterator itU = users.begin(); itU < users.end(); itU++)
				if (cUser.getFd() != -1)
					_rep.R353(cUser.getFd(), cUser.getNick(), itChannel->getName(), itU->getNick(),itChannel->getChanPrefix(), itChannel->getUserPrefix(*itU));
			_rep.R366(cUser.getFd(), cUser.getNick(), itChannel->getName());
            users.clear();
		}
    }
}
