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

void Rooms::set_price(int price_)
{
    price = price_;
}

void Rooms::set_maxcap(int maxCapacity_)
{
    maxCapacity = maxCapacity_;
}

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

void Rooms::set_price(int price_)
{
    price = price_;
}

void Rooms::set_maxcap(int maxCapacity_)
{
    maxCapacity = maxCapacity_;
}

void Rooms::set_capacity(int capacity_)
{
    capacity = capacity_;
}

void Rooms::set_userInRooms(std::vector<userInRoom> userInRooms_)
{
    userInRooms = userInRooms_;
}
