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

#define ERR230 "230: User logged in."
#define ERR231 "231: User successfully signed up."
#define ERR311 "311: User Signed up. Enter your password, purse, phone and address."
#define ERR403 "403: Access denied!"
#define ERR503 "503: Bad sequence of commands."

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
