#ifndef READJSON_HPP_
#define READJSON_HPP_ "READJSON_HPP_"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "json.hpp"
#include "user.hpp"
#include "admin.hpp"
#include "rooms.hpp"

using json = nlohmann::json;

class readJson
{
public:
    readJson();
    std::vector<User*> users;
    std::vector<Admin*> admins;
    std::vector<Rooms*> rooms;
    int getPort();
    std::string getHostName();
private:
    std::string hostName;
    int port;
};

#endif
