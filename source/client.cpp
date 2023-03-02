#include "../library/client.hpp"

Client::Client() {}
void show_list(std::vector<User *> &users);

void signin(std::string username, std::string password, std::vector<User*>& users)
{
    for (int i = 0; i < users.size(); i++)
    {
        if (users[i]->getname() == username && users[i]->getpassword() == password)
        {
            std::cout << "230: User logged in." << std::endl;
            return;
        }
    }
    std::cout << "430: Invalid username or password." << std::endl;
    return;
}

bool checkIsANumber(std::string input, std::vector<User*>& users)
{
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] < '0' || input[i] > '9')
        {
            std::cout << "503: Bad sequence of commands." << std::endl;
            return false;
        }
    }
    return true;
}

void signup(std::string name, std::vector<User*>& users)
{
    //check if the user already exists
    bool isExisted = false;
    for (int i = 0; i < users.size();i++)
    {
        std::string username = users[i]->getname();
        if (username == name)
        {
            std::cout << "451: User already existed!" << std::endl;
            isExisted = true;
            show_list(users);
            return;
        }
    }

    //create a new user
    if (!isExisted)
    {
        std::cout << "311: User Signed up. Enter your password, purse, phone and address." << std::endl;
        std::string password, purse, phoneNumber, address;
        std:: cout << "Password: ";
        std::cin >> password;
        std:: cout << "Purse: ";
        std::cin >> purse;
        if (checkIsANumber(purse, users) == false)
        {
            return;
        }
        std:: cout << "Phone Number: ";
        std::cin >> phoneNumber;
        if (checkIsANumber(phoneNumber, users) == false)
        {
            return;
        }
        std:: cout << "Address: ";
        std::getline(std::cin >> std::ws, address);
        users.push_back(new User(users.size(), name, password, purse, phoneNumber, address));
        std::cout << "231: User successfully signed up." << std::endl;
    }
}

void define_command(std::string command, std::vector<User*>& users)
{
    std::stringstream ss(command);
    std::string order,word;
    std::getline (ss, order, ' ');

    if(order == "signin")
    {
        std::getline (ss, word, ' ');
        std::string username = word;
        std::getline (ss, word, ' ');
        std::string password = word;
        signin(username, password, users);
    }
    else if(order == "signup")
    {
        std::getline (ss, word, ' ');
        signup(word, users);
    }
    else
        std::cout << "503: Bad sequence of commands." << std::endl;
}

void show_list(std::vector<User*>& users)
{
    while(true)
    {
        std::cout << "Welcome! Please choose one of the following commands:" << std::endl;
        std::cout << "signin <username> <password>" << std::endl;
        std::cout << "signup <username>" << std::endl;

        std::string str;
        std::getline(std::cin >> std::ws, str);
        define_command(str, users);
    }
}

int main(int argc, char const *argv[])
{
    readJson data;
    std::vector<User*> users = data.getUsers();
    Client client;
    show_list(users);
    return 0;
}