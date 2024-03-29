/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psaulnie <psaulnie@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 11:00:07 by psaulnie          #+#    #+#             */
/*   Updated: 2023/03/14 10:44:09 by psaulnie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/Server.hpp"


Server::Server(int port, std::string password) : _rep(*this), _connected_clients(0), _port(port), _password(password) { }

Server::~Server() { }

void	Server::starting()
{
	/****************************************************************************************************\
	*	Definition from GNU docs:																		 *
	*		A socket is a generalized interprocess communication channel.								 *
	*		Like a pipe, a socket is represented as a file descriptor.									 *
	*		Unlike pipes sockets support communication between unrelated processes,						 *
	*		and even between processes running on different machines that communicate over a network. 	 *
	\****************************************************************************************************/

	/*
		socket(): Creating the server socket:
			AF_INET: specifying using the IP protocol
			SOCK_STREAM: dialog support guarranting an integrity
	*/

	this->_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_server_fd < 0)
	{
		std::perror("socket");
		throw std::exception();
	}

	/*
		setsocketopt(): Setting option to the socket:
			SOL_SOCKET: level arg, the socket layer itself
			SO_REUSEADDR: controls if bind() should permit to reuse local address for this socket
			SO_REUSEPORT: same as SO_REUSEADDR but for the port
			opt: setting to 1 the aboves parameters
	*/

	int opt = 1;
	if (setsockopt(this->_server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
	{
		std::perror("setsocketopt");
		throw std::exception();
	}

	this->_address.sin_family = AF_INET; // IPv4 protocol
	this->_address.sin_addr.s_addr = INADDR_ANY; // Accepting all addresses
	this->_address.sin_port = htons(this->_port); // Used port
	
	/*
		bind(): set a "name" to the socket
			Assigns the address specified to the socket
	*/

	if (bind(_server_fd, (struct sockaddr*)&_address, sizeof(_address)) < 0)
	{
		std::perror("bind");
		throw std::exception();
	}

	/*
		listen(): set the socket to passive, meaning it will be used to accept incoming connections
			MAX_INCONNECTIONS: max length of the pending connections line, if there is too many connections, the client will receive a ECONNREFUSED error
	*/

	if (listen(this->_server_fd, MAX_INCONNECTIONS) < 0)
	{
		std::perror("listen");
		throw std::exception();
	}

	initCommands(); // Setting the array of commands used in commandHandler()

	// Initializing the array of connected clients
	for (int i = 0; i < MAX_CONNECTIONS; i++)
	{
		User	tmp_user = User();
		_clients.push_back(tmp_user);
	}
	_clients[0].setFd(_server_fd);

	// Getting the time of creation of the server

	char date_string[128];
	time_t curr_time;
	tm *curr_tm;
	_date = std::time(&curr_time);
	curr_tm = std::localtime(&curr_time);

	std::strftime(date_string, 50, "%c", curr_tm);

	_date = date_string;
}

/*
	Command init template:
		_commands.insert(ft::make_pair(std::string(""), &Server::)); 
*/

void	Server::initCommands()
{
	_commands.insert(std::make_pair(std::string("DIE"), &Server::dieCmd));
	_commands.insert(std::make_pair(std::string("INVITE"), &Server::inviteCmd));
	_commands.insert(std::make_pair(std::string("JOIN"), &Server::joinCmd));
	_commands.insert(std::make_pair(std::string("KICK"), &Server::kickCmd));
	_commands.insert(std::make_pair(std::string("MODE"), &Server::modeCmd));
	_commands.insert(std::make_pair(std::string("MSG"), &Server::msgCmd));
	_commands.insert(std::make_pair(std::string("NICK"), &Server::nickCmd));
	_commands.insert(std::make_pair(std::string("NOTICE"), &Server::msgCmd));
	_commands.insert(std::make_pair(std::string("PART"), &Server::partCmd));
	_commands.insert(std::make_pair(std::string("PASS"), &Server::passCmd));
	_commands.insert(std::make_pair(std::string("PRIVMSG"), &Server::msgCmd));
	_commands.insert(std::make_pair(std::string("QUIT"), &Server::quitCmd));
	_commands.insert(std::make_pair(std::string("TOPIC"), &Server::topicCmd));
	_commands.insert(std::make_pair(std::string("USER"), &Server::userCmd));
	_commands.insert(std::make_pair(std::string("KILL"), &Server::killCmd));
}

void	Server::run()
{
	fd_set	read_fd_set;
	fd_set	write_fd_set;
	int		rvalue;

	std::cout << "The IRC server is running." << std::endl << "Waiting for connections..." << std::endl;
	while (true)
	{
		// An FD list is used by select to work, it contains all the connected fd and need to be refilled everytime it loops
		FD_ZERO(&read_fd_set); //  Cleaning the FD list before filling it
		FD_ZERO(&write_fd_set);
		for (int i = 0; i < MAX_CONNECTIONS; i++)
		{
			if (_clients[i].getFd() >= 0)
			{
				FD_SET(_clients[i].getFd(), &read_fd_set); // Reading all the connected clients to the list
				FD_SET(_clients[i].getFd(), &write_fd_set);
			}
		}
		FD_SET(0, &read_fd_set);
		FD_SET(0, &write_fd_set);

		// select(): check a list of FD to see if one need an operation
		rvalue = select(FD_SETSIZE, &read_fd_set, &write_fd_set, NULL, NULL);
		if (rvalue < 0)
		{
			std::perror("select");
			throw std::exception();
		}

		if (FD_ISSET(_server_fd, &read_fd_set) && _connected_clients < MAX_CONNECTIONS) // If the server fd is triggered, new connection to the server
			acceptClient();
		else if (_connected_clients >= MAX_CONNECTIONS)
			std::cerr << "Too many clients. Refusing incoming connection" << std::endl;
		for (int i = 1; i < MAX_CONNECTIONS; i++) // Checking on all connections if one is triggered
		{
			if (_clients[i].getFd() > 0 && FD_ISSET(_clients[i].getFd(), &read_fd_set)) // If one is triggered, handle it
				manageClient(i);
			if (_clients[i].getFd() > 0 && _clients[i].getCanRecv() && FD_ISSET(_clients[i].getFd(), &write_fd_set))
				sendToClient(i);
		}
	}
}

void	Server::acceptClient()
{
	int							new_connection;
    std::string					tmp;
	User						tmp_user;

	// accept(): extract the first pending connection in the list and create a new socket
	new_connection = accept(_server_fd, (struct sockaddr *)&tmp_user.getAdress(), &tmp_user.getAdressLen());
	if (new_connection < 0)
	{
		std::perror("accept");
		throw std::exception();
	}
	for (int i = 0; i < MAX_CONNECTIONS; i++)
	{
		if (_clients[i].getFd() < 0)
		{
			_clients[i].setFd(new_connection);
			_clients[i].setRegister(false);
			_clients[i].setRPassword(false);
			if (_connected_clients == 0) {
				_clients[i].setIrcOp(true);
			}
			break ;
		}
	}
	_connected_clients++;
	std::cout << "Connected clients: " << RED << _connected_clients << NO_COLOR << std::endl;
}

void	Server::manageClient(int &index)
{
	int			rvalue;

	rvalue = receive(_clients[index].getFd());
	if (rvalue == 0)
	{
		std::vector<std::string>	tmp;
		quitCmd(tmp, _clients[index]);
	}
}


void		Server::commandHandler(std::string const &output, int const &current)
{
	std::vector<std::string>	parsed_output;
	std::string					tmp;
	int							user_index;

	for (user_index = 0; user_index < MAX_CONNECTIONS; user_index++)
		if (_clients[user_index].getFd() == current)
			break ;

	if (user_index == MAX_CONNECTIONS)
	{
		std::cerr << "internal error: not finding the associated fd" << std::endl;
		throw std::exception();
	}

	// Splitting the commands by parameters
	for (size_t i = 0; i < output.length(); i++)
	{
		char c = output[i];
		if (std::isspace(c))
		{
			parsed_output.push_back(tmp);
			tmp.clear();
		}
		else
			tmp.push_back(c);
	}
	if (tmp != "")
		parsed_output.push_back(tmp);
	std::cout << CYAN << "Received command: " << output << NO_COLOR << std::endl;
	if (parsed_output.size() == 0)
		return ;
	if (parsed_output[0] == "PING" && parsed_output.size() > 1) // Needed for weechat lag
		emit("PONG " + parsed_output[1], current);
	// Find the command by his name, needs to be registered to use them excepts the necessary commands to log in
	if (_commands.find(parsed_output[0]) != _commands.end())
	{
		if ((_clients[user_index].getRegister() && _clients[user_index].getRPassword()) || parsed_output[0] == "PASS" || parsed_output[0] == "NICK" || parsed_output[0] == "USER")
		{
			(this->*_commands[parsed_output[0]])(parsed_output, _clients[user_index]); // Execute command corresponding to the input
		}
		else
			_rep.E451(_clients[user_index].getFd(), _clients[user_index].getNick());
	}
	else if (parsed_output[0] != "PING" && parsed_output[0] != "CAP")
		_rep.E421(_clients[user_index].getFd(), _clients[user_index].getNick(), parsed_output[0]);
}

void	Server::shutdown()
{
	std::cout << "Shutting down the server..." << std::endl;
	for (std::vector<User>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->getFd() != -1)
			close(it->getFd());
	}
	close(_server_fd);
	_channels.clear();
	_clients.clear();
	_commands.clear();
}

void	Server::emit(std::string const &input, int const &fd) const
{
	std::string	msg = input + "\r\n";
	int 		error;

	std::cout << "Message sent: " << input << std::endl;
	error = send(fd, msg.c_str(), msg.size(), 0);
	if (error < 0)
	{
		std::perror("send");
		throw std::exception();
	}
}

int		Server::receive(int const &fd)
{
	char	buffer[1024 + 1];
	int		rvalue;
	std::vector<User>::iterator itClient;

	for (itClient = _clients.begin(); itClient != _clients.end(); itClient++)
	{
		if (itClient->getFd() == fd)
			break ;
	}
	std::memset(&buffer, 1, 1024);
	rvalue = recv(fd, &buffer, 1024, 0);
	if (rvalue < 0)
	{
		std::perror("recv");
		throw std::exception();
	}
	if (rvalue == 0)
	{
		return (0);
	}
	if (buffer[rvalue] == 0 || buffer[rvalue - 1] == 10)
	{
		buffer[rvalue] = 0;
		itClient->appendToMsg(buffer);
		itClient->setCanRecv(true);
		return (1);
	}
	buffer[rvalue] = '\0';
	itClient->appendToMsg(buffer);
	return (1);
}

void	Server::sendToClient(int &index)
{
	std::string	&output = _clients[index].getMsg();
	// Split the output of receive() in case multiples commands are received
	size_t pos = 0;
	std::string token;
	std::string	delimiter = "\n" ;
	while ((pos = output.find(delimiter)) != std::string::npos)
	{
		token = output.substr(0, pos);
		commandHandler(token, _clients[index].getFd());
		output.erase(0, pos + delimiter.length());
	}
	_clients[index].setCanRecv(false);
}