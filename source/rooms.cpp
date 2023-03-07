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

std::string Rooms::getnum()
{
    return number;
}

int Rooms::getstatus()
{
    return status;
}

int Rooms::getprice()
{
    return price;
}

int Rooms::getmax_capacity()
{
    return maxCapacity;
}

int Rooms::getcapacity()
{
    return capacity;
}

std::vector<userInRoom> Rooms::getusers()
{
    return userInRooms;
}

