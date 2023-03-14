#ifndef CLIENT_HPP
#define CLIENT_HPP "CLIENT_HPP"

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sstream>
#include <unistd.h>
#include <chrono>
#include <ctime>
#include <sys/stat.h>//for mkdir
#include "readJson.hpp"
#include "error.hpp"

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
