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

void User::cash_back(int cash)
{
    int purse_int = std::stoi(purse);
    purse_int += cash;
    purse = std::to_string(purse_int);
}

std::string User::get_info(bool pass_needed)
{
    std::stringstream ss;
    if(pass_needed) ss << "###########################" << std::endl;
    ss << "id: " << std::to_string(id) << std::endl;
    ss << "name: " << user << std::endl;
    if(pass_needed) ss << "password: " << password << std::endl;
    ss << "purse: " << purse << std::endl;
    ss << "phoneNumber: " << phoneNumber << std::endl;
    ss << "address: " << address << std::endl;
    ss << "###########################" << std::endl;
    if(pass_needed) ss << "/" << id <<"/user";
    return ss.str();
}
