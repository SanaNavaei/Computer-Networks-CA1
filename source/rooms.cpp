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

void Rooms::set_capacity(int capacity_)
{
    capacity = capacity_;
}

void Rooms::set_userInRooms(std::vector<userInRoom> userInRooms_)
{
    userInRooms = userInRooms_;
}

void Rooms::set_status(int status_)
{
    status = status_;
}

void Rooms::add_user(int id_, std::string numOfBeds_, std::string reserveDate_, std::string checkoutDate_)
{
    userInRoom new_user;
    new_user.id = id_;
    new_user.numOfBeds = stoi(numOfBeds_);
    new_user.reserveDate = reserveDate_;
    new_user.checkoutDate = checkoutDate_;
    userInRooms.push_back(new_user);
}

void Rooms::change_capacity(int reduce_by)
{
    capacity -= reduce_by;
}
