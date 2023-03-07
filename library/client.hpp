#ifndef CLIENT_HPP
#define CLIENT_HPP "CLIENT_HPP"

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sstream>
#include <unistd.h>
#include "readJson.hpp"

#define ERR230 "230: User logged in."
#define ERR311 "311: User Signed up. Enter your password, purse, phone and address."
#define ERR503 "503: Bad sequence of commands."
#define ERR403 "403: Access denied!"

class Client
{
public:
    Client();
    void build();
    int connectServer(int port);
private:
    readJson data;
};

#endif
