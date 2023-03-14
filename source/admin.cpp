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

void Admin::setpassword(std::string password_)
{
    password = password_;
}

std::string Admin::get_info()
{
    std::stringstream ss;
    ss << "###########################" << std::endl;
    ss << "id: " << std::to_string(id) << std::endl;
    ss << "name: " << user << std::endl;
    ss << "password: " << password << std::endl;
    ss << "###########################" << std::endl;
    ss << "/" << id <<"/admin/1";
    return ss.str();
}
