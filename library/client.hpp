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

class Client
{
public:
    Client();
private:
    readJson data;
};

#endif