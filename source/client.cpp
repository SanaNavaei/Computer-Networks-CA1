#include "../library/client.hpp"

Client::Client() {}
void show_list(std::vector<User *> &users);

void action_to_be_done(int choice)
{
    switch(choice)
    {
        case 1:
            //View user information
            break;
        case 2:
            //View all users
            break;
        case 3:
            //View rooms information
            break;
        case 4:
            //Booking
            break;
        case 5:
            //Canceling
            break;
        case 6:
            //pass day
            break;
        case 7:
            //Edit information
            break;
        case 8:
            //Leaving room
            break;
        case 9:
            //Rooms
            break;
        case 0:
            //Logout
            break;
    }
}
bool isNumberBetween0And9(std::string str) {
    // Check if the string only contains one character and that it is a digit
    if (str.length() == 1 && isdigit(str[0])) {
        int num = stoi(str);
        // Check if the number is between 0 and 9
        if (num >= 0 && num <= 9) {
            return true;
        }
    }
    return false;
}

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
        if(!define_command(str, users))
            continue;
        while(true)
        {
            std::cout << "Hi! Choose one of the items below by just entering the number of that.\n";
            std::cout << "1. View user information\n";
            std::cout << "2. View all users\n";
            std::cout << "3. View rooms information\n";
            std::cout << "4. Booking\n";
            std::cout << "5. Canceling\n";
            std::cout << "6. pass day\n";
            std::cout << "7. Edit information\n";
            std::cout << "8. Leaving room\n";
            std::cout << "9. Rooms\n";
            std::cout << "0. Logout\n";
            std::cout << "--> <choice number> :\n";
            std::string choice_num;
            std::getline(std::cin >> std::ws, choice_num);
            if (!isNumberBetween0And9(choice_num))
            {
                std::cout << "503: Bad sequence of commands." << std::endl;
                continue;               
            }
            else
            {
                action_to_be_done(stoi(choice_num));
            }
        }
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
