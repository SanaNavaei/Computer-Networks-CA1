#include "../library/admin.hpp"

Admin::Admin(int id_, std::string user_, std::string password_)
{
    id = id_;
    user = user_;
    password = password_;
}

std::string Admin::getname()
{
    return user;
}

std::string Admin::getpassword()
{
    return password;
}

int Admin::getid()
{
    return id;
}
