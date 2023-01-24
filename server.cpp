// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <map>
#include "User.hpp"
#define PORT 8080
int main(int argc, char const* argv[])
{
	std::map<std::string, User>	users;
    std::string					tmp;
	User						tmp_user;
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };
	
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
 
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
 
 while (1)
 {
	 /* code */
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
 
    if ((new_socket
         = accept(server_fd, (struct sockaddr*)&address,
                  (socklen_t*)&addrlen))
        < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
	bzero(buffer, 1024);
	while (recv(new_socket, buffer, 1024, 0) >= 0)
	{
		tmp = buffer;
    	// printf("[%s]\n", buffer);
		std::cout << tmp << std::endl;
		int	size;
		if ((size = tmp.find("NICK ", 0)) == 0)
			tmp_user.setNick(tmp.substr(5));
		if ((size = tmp.find("USER ", 0)) == 0)
		{
			std::cout << tmp.find(" ", 5) << std::endl;
			tmp_user.setUser(tmp.substr(5, tmp.find(" ", 5) - 5));
			break ;
		}
		
	}

	std::cout << "salut " << tmp_user.getUser() << "]" <<  std::endl;
	users[tmp_user.getNick()] = tmp_user;
	std::string	msg = "001 " + tmp_user.getNick() + ":Welcome";// to the Internet Relay Network " + tmp_user.getNick() + "!" + tmp_user.getUser() + "@" + "127.0.0.1";

    send(new_socket, msg.c_str(), strlen(msg.c_str()), 0);
    // valread = recv(new_socket, buffer, 1024, NULL);
    // valread = recv(new_socket, buffer, 1024, NULL);
    // printf("%s\n", buffer);
    // valread = recv(new_socket, buffer, 1024, NULL);
    // printf("%s\n", buffer);
 }
 
    // closing the connected socket
    close(new_socket);
    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
    return 0;
}