#ifndef SERVER_HPP_
#define SERVER_HPP_ "SERVER_HPP"

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sstream>
#include <unistd.h>
#include "readJson.hpp"

#define MAX_CLIENTS 10
#define ERR101 "101: The desired room was not found."
#define ERR104 "104: Successfully added."
#define ERR105 "105: Successfully modified."
#define ERR106 "106: Successfully deleted."
#define ERR109 "109: The room capacity is full."
#define ERR111 "111: This room already exists."
#define ERR201 "201: User logged out successfully."
#define ERR230 "230: User logged in."
#define ERR231 "231: User successfully signed up."
#define ERR311 "311: User Signed up. Enter your password, purse, phone and address."
#define ERR312 "312: Information was changed successfully."
#define ERR403 "403: Access denied!"
#define ERR401 "401: Invalid value!"
#define ERR430 "430: Invalid username or password."
#define ERR451 "451: User already existed!"
#define ERR503 "503: Bad sequence of commands."
#define DELIM '-'

struct Date {
    std::string day;
    std::string month;
    std::string year;
};

class Server
{
public:
    Server(readJson data);
    
    int setup_server(int port);
    
    void build();
    void signup(std::string username, std::string password, std::string purse, std::string phoneNumber, std::string address, int fd);
    void checkCommand(char buff[], int fd);
    void set_date(std::string day_, std::string month_, std::string year_);
    void checkusername(std::string name, int fd);
    void signin(std::string username, std::string password, int fd);
    void action_to_be_done(int choice, int id, int fd, std::istringstream& ss);
    void logout(int id, int fd);
    void edit_information(int id, int fd, std::istringstream &ss);
    void edit_rooms(int id, int fd, std::istringstream &ss);

    std::string get_info(int id);
    std::string user_info_gathering(int id);

    bool checkDateFormat(const std::string& input);
    bool checkIsANumber(std::string input, int fd);
    bool check_if_is_admin(int id);
    bool check_room_exist(std::string room_number);
private:
    
    readJson data;
    Date sys_date;
    std::vector<int> loggedInIds;
};

#endif
