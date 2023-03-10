/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaulnie <psaulnie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 13:12:10 by psaulnie          #+#    #+#             */
/*   Updated: 2023/03/07 13:46:02 by psaulnie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Server.hpp"
#include <stdexcept>

int main(int argc, char *argv[])
{
	int	port;

	if (argc != 3)
	{
		std::cout << "Usage: ./ircserv [port] [password]" << std::endl;
		return (-1);
	}
	for (int i = 0; argv[1][i]; i++)
	{
		if (!std::isdigit(argv[1][i]))
		{
			std::cout << "First parameter must be a number between 1 and 65535" << std::endl;
			return (1);
		}
	}
	port = std::atoi(argv[1]);
	if (port < 1 || port > 65535)
	{
		std::cout << "Port range must be between 1 and 65535" << std::endl;
		return (-1);
	}

	Server	server(port, argv[2]);

	try
	{
		server.starting();
		server.run();
	}
	catch(const std::exception& e)
	{
		server.shutdown();
		return (1);
	}

	std::cout << "Shutting down the server" << std::endl;
	return (0);
}
