#ifndef ADMIN_HPP_
#define ADMIN_HPP_ "ADMIN_HPP_"

#include <string>
#include <vector>

class Admin
{
public:
    Admin(int id_, std::string user_, std::string password_);
    std::string getname();
    std::string getpassword();
    int getid();
private:
    int id;
    std::string user;
    std::string password;
};

#endif
