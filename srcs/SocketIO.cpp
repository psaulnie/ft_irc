/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketIO.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaulnie <psaulnie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 13:45:14 by psaulnie          #+#    #+#             */
/*   Updated: 2023/02/03 17:02:50 by psaulnie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/SocketIO.hpp"

SocketIO::SocketIO() { }

SocketIO::~SocketIO() { }

void	SocketIO::emit(std::string const &input, int fd) const
{
	if (send(fd, input.c_str(), input.length(), 0) < 0)
	{
		std::cout << "send: error" << std::endl; // TODO explicit msg
		throw std::exception();
	}		
}

int	SocketIO::receive(std::string &output, int fd) const
{
	char	buffer[1024];
	int		rvalue;

	while ((rvalue = recv(fd, &buffer, 1024, 0)) > 0)
	{
		buffer[rvalue] = '\0';
		output += buffer;
	}
	if (rvalue < 0 && errno == EBADFD)
	{
		std::cout << "recv: error" << std::endl; // TODO explicit msg
		throw std::exception();
	}
	// buffer[rvalue] = '\0';
	// output = buffer;
	return (rvalue);
}
