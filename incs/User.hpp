/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaulnie <psaulnie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 10:58:58 by psaulnie          #+#    #+#             */
/*   Updated: 2023/03/14 10:22:06 by psaulnie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include <vector>
# include <netinet/in.h>
# include <unistd.h> 
# include "Channel.hpp"
# define MAX_CHAN 50

class Channel;

class User
{
	private:
		char					_buffer[1024];
		std::string				_msg;
		int						_fd;
		std::string				_nick;
		std::string				_user;
		std::string				_prefix;
		std::map<char, bool>	_mode;
		std::vector<Channel>	_voicedChan;
		std::vector<Channel>	_inviteChan;
		std::vector<Channel>	_chanOp; // Array of channels in which the user is operator
		bool					_ircOp; // true: user is irc operator, false: user is not irc operator
		bool					_is_registered;
		bool					_right_password;
		bool					_unused_nick;
		struct sockaddr_in		_address;
		socklen_t				_addrlen;
		int						_chanConnected;
		bool					_initConv;
		bool					_emitMsg;
	public:
		User();
		User(const std::string &nick, const std::string &user);
		User(const User &src);
		virtual ~User();

		User &operator=(const User &rhs);
		bool operator==(const User &rhs) const;
		bool operator!=(const User &rhs) const;

		void	setFd(int new_fd);
		void	setNick(const std::string &nick);
		void	setUser(const std::string &user);
		void	setPrefix(const std::string &prefix);
		void 	setMode(char const &modeName, bool const &isMode);
		void	addOpChannel(Channel &channel);
		void	removeOpChannel(Channel &channel);
		void	addVoicedChan(Channel &voicedChan);
		void	removeVoicedChannel(Channel &voicedChan);
		void	addInviteChan(Channel &inviteChan);
		void	removeInviteChan(Channel &inviteChan);
		void	setIrcOp(bool ircOp);
		void	setRegister(bool const &input);
		void	setRPassword(bool const &input);
		void	setUnusedNick(bool const &input);
		void	setCanRecv(bool const &set);
		void	appendToMsg(char *buffer);

		void 	incrChanConnected();
		void	decrChanConnected();
		void	resetUser();
		void	setInitConv(bool value);

		int const					&getFd() const;
		std::string const			&getNick() const;
		bool						isMode(char mode);
		std::string					getModes();
		bool 						isIrcOp() const;
		bool 						isChanOp(Channel &channel);
		bool						isVoicedChan(Channel &channel);
		bool						isInviteChan(Channel &inviteChan);
		bool const					&getRegister() const;
		bool const					&getRPassword() const;
		bool const					&getUnusedNick() const;
		bool const					&getCanRecv() const;
		struct sockaddr_in			&getAdress();
		socklen_t					&getAdressLen();
		int const 					&getChanConnected() const;
		bool						isInitConv();
		std::string					&getMsg();
};

#endif
