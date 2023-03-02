#include "../library/rooms.hpp"

Rooms::Rooms(std::string number_, int status_, int price_, int maxCapacity_, int capacity_, std::vector<userInRoom> userInRooms_)
{
    number = number_;
    status = status_;
    price = price_;
    maxCapacity = maxCapacity_;
    capacity = capacity_;
    userInRooms = userInRooms_;
}
