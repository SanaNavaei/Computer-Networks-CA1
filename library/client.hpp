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

class Client
{
public:
    Client();
    void build();
private:
    readJson data;
};

#endif
