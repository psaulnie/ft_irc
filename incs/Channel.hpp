/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbattest <lbattest@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 11:43:58 by dbouron           #+#    #+#             */
/*   Updated: 2023/02/17 12:38:58 by lbattest         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <map>
# include <vector>
# include "User.hpp"

class User;

class Channel
{
	public:
		Channel(const std::string &name, User &opUser);
		Channel(const Channel &src);
		Channel &operator=(const Channel &rhs);
		virtual ~Channel();

		const std::string		&getName() const;
		const std::string		&getSubject() const;
		bool					isMode(char mode);
		const std::vector<User>	&getUsers() const;
		bool					isUser(User &user);
		bool					isOpUser(User &user);
		bool					isBanned(User &user);
		const std::string		getPw() const;
		unsigned short			getUsrCon() const;
		unsigned short			getUsrNbMax() const;
		bool					getIsTopic() const;

		void setName(const std::string &name);
		void setSubject(const std::string &subject);
		void setMode(char const &modeName, bool const &isMode);
		void addUser(User &user);
		void removeUser(User &user);
		void addOpUser(User &opUser);
		void banUser(User &user);
		void setPw(std::string pw);
		void incrUsrCon();
		void setUsrNbMax(unsigned short nbr);
		void setIsTopic(bool status);
		bool operator==(const Channel &rhs) const;
		bool operator!=(const Channel &rhs) const;
		
	private:
		std::string				_name; //200 char max
		std::string 			_subject;
		bool					_isTopic;
		std::map<char, bool>	_mode;
		std::vector<User>		_users;
		std::vector<User>		_opUsers;
		std::vector<User>		_bannedUsers;
		unsigned short			_usrNbMax;
		unsigned short			_usrCon;
		std::string 			_pw;

		Channel();
};

#endif //CHANNEL_HPP