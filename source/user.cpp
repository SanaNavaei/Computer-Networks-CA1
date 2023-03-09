#include "../library/user.hpp"

User::User(int id_, std::string user_, std::string password_, std::string purse_, std::string phoneNumber_, std::string address_)
{
    id = id_;
    user = user_;
    password = password_;
    purse = purse_;
    phoneNumber = phoneNumber_;
    address = address_;
}

std::string User::getname()
{
    return user;
}

std::string User::getpassword()
{
    return password;
}

int User::getid()
{
    return id;
}

std::string User::getaddress()
{
    return address;
}

std::string User::getphone()
{
    return phoneNumber;
}

std::string User::getpurse()
{
    return purse;
}

void User::setpassword(std::string password_)
{
    password = password_;
}

void User::setaddress(std::string address_)
{
    address = address_;
}

void User::setphoneNumber(std::string phoneNumber_)
{
    phoneNumber = phoneNumber_;
}

void User::setpurse(std::string purse_)
{
    purse = purse_;
}
