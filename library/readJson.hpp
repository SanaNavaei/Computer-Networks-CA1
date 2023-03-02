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
    int getPort();
private:
    std::string hostName;
    std::vector<User*> users;
    std::vector<Admin*> admins;
    std::vector<Rooms*> rooms;
    int port;
};

#endif
