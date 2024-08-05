#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <map>
#include <vector>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Utilities.hpp"
#include "Exception.hpp"
#include "Replies.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include <cerrno>
#include <cstring>

class Server;
class Client;

typedef int (Server::*command)(std::string& , Client&);

class Server
{
    private:
        size_t                  port_number;
        const std::string       password;
        size_t                  server_fd;
        sockaddr_in             addr;
        std::vector<Client>     clients;
        char                    buffer[1024];
        int                     state;

        fd_set readFds;
        fd_set writeFds;
        fd_set readFdsSup;
        fd_set writeFdsSup;

    private:
        // create socket
        void createSocket(void);
        void serverAddrSocket(void);
        void socketListen(void) const;
        void run(void);
        int findMaxFd(void) const;
        void acceptClient();
        void readEvent();
        void writeEvent();

    public:
        Server();
        Server(size_t port_number, const std::string password);
        ~Server();

        Client* find_client(std::string &nick);


        void cmds_initialize(void);

        std::map<std::string, command> cmds;
        int Nick(std::string &input, Client& c);//zorunlu
        int Cap(std::string &input, Client& c);//karsi tarafin hexchatten baglandigini gosteren sey
        int Privmsg(std::string &input, Client& c);//operator
        int Join(std::string &input, Client& c);//zorunlu
        int Pass(std::string &input, Client& c);//zorunlu
        int User(std::string &input, Client& c);//zorunlu
        int Topic(std::string &input, Client& c);//operator
        int Notice(std::string &input, Client& c);//operator
        int Info(std::string &input, Client& c);
        int Invite(std::string &input, Client& c);
        int Part(std::string &input, Client& c);
        int Pong(std::string &input, Client& c);
        int Quit(std::string &input, Client& c);
        int Mode(std::string &input, Client& c);
        int Kick(std::string &input, Client& c);
};