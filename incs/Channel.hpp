/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaulnie <psaulnie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 11:43:58 by dbouron           #+#    #+#             */
/*   Updated: 2023/03/03 10:21:44 by psaulnie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <map>
# include <vector>
# include "User.hpp"
# include "NumericReplies.hpp"

# define PURPLE "\033[0;35m"
# define GREEN "\033[0;32m"
# define NO_COLOR "\033[0m"

class User;

class Channel
{
	public:
		Channel(const std::string &name, User &opUser);
		Channel(const Channel &src);
		Channel &operator=(const Channel &rhs);
		virtual ~Channel();

		const std::string	&getName() const;
		const std::string	&getSubject() const;
		bool				isMode(char mode);
		std::string			getModes();
		std::vector<User>	&getUsers();
		std::vector<User>	&getOpUsers();
		bool				isUser(User &user);
		bool				isOpUser(User &user);
		bool				isBanned(User &user);
		const std::string	getPw() const;
		unsigned short		getUsrCon() const;
		unsigned short		getUsrNbMax() const;
		bool				getIsTopic() const;
		char				getChanPrefix();
		char			 	getUserPrefix(User &cUser);
		void setSubject(const std::string &subject);
		void setMode(char const &modeName, bool const &isMode);
		void addUser(User &user);
		void removeUser(User &user);
		void removeOpUser(User &user);
		void addOpUser(User &opUser);
		void banUser(User &user);
		void unbanUser(User &user);
		void listBannedUser(Rep &rep, User const &cUser);
		void setPw(std::string pw);
		void incrUsrCon();
		void decrUsrCon();
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

#endif // CHANNEL_HPP
