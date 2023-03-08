#ifndef ROOMS_HPP_
#define ROOMS_HPP_ "ROOMS_HPP_"

#include <string>
#include <vector>

struct userInRoom
{
    int id;
    int numOfBeds;
    std::string reserveDate;
    std::string checkoutDate;
};

class Rooms
{
public:
    Rooms(std::string number_, int status_, int price_, int maxCapacity_, int capacity_, std::vector<userInRoom> userInRooms_);
    std::string getnum();
    
    int getstatus();
    int getprice();
    int getmax_capacity();
    int getcapacity();
    
    std::vector<userInRoom> getusers();
    
    void set_price(int price_);
    void set_maxcap(int maxCapacity_);
    void set_capacity(int capacity_);
    void set_userInRooms(std::vector<userInRoom> userInRooms_);
    void set_status(int status_);
private:
    std::string number;
    int status;
    int price;
    int maxCapacity;
    int capacity;
    std::vector<userInRoom> userInRooms;
};

#endif
