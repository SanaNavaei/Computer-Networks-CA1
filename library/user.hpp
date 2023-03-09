#ifndef USER_HPP_
#define USER_HPP_ "USER_HPP"

#include <string>

class User
{
public:
    User(int id_, std::string user_, std::string password_, std::string purse_, std::string phoneNumber_, std::string address_);
    
    std::string getname();
    std::string getpassword();
    std::string getphone();
    std::string getaddress();
    std::string getpurse();
    
    void setpassword(std::string password_);
    void setaddress(std::string address_);
    void setphoneNumber(std::string phoneNumber_);
    void setpurse(std::string purse_);
    void cash_back(int cash);
    
    int getid();
private:
    int id;
    std::string user;
    std::string password;
    std::string purse;
    std::string phoneNumber;
    std::string address;
};

#endif
