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
