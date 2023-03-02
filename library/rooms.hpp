#ifndef ROOMS_HPP_
#define ROOMS_HPP_ "ROOMS_HPP_"

#include <string>
#include <vector>

class Rooms
{
public:
    Rooms(std::string number_, int status_, int price_, int maxCapacity_, int capacity_, std::vector<userInRoom> userInRooms_);
private:
    std::string number;
    int status;
    int price;
    int maxCapacity;
    int capacity;
    std::vector<userInRoom> userInRooms;
};

#endif
