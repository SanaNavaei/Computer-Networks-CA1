#ifndef SERVER_HPP_
#define SERVER_HPP_ "SERVER_HPP"

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include "readJson.hpp"
class Server
{
public:
    Server(readJson data);
    void build();
    int setup_server(int port);
private:
    readJson data;
};
#endif
