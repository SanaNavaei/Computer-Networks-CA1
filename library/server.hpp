#ifndef SERVER_HPP_
#define SERVER_HPP_ "SERVER_HPP"

#include <iostream>
#include "readJson.hpp"
class Server
{
public:
    Server(readJson data);
    bool build();
    int setup_server(int port);
private:
    readJson data;
};
#endif
