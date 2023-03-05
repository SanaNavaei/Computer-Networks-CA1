#include "../library/client.hpp"

Client::Client() {}

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

std::string user_list()
{
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
            return "menu/" + choice_num;
        }
    }
    return "";
}

std::string get_data_signup(std::string new_username)
{
    std::string password, purse, phoneNumber, address;
    std:: cout << "Password: ";
    std::cin >> password;
    std:: cout << "Purse: ";
    std::cin >> purse;
    std:: cout << "Phone Number: ";
    std::cin >> phoneNumber;
    std:: cout << "Address: ";
    std::getline(std::cin >> std::ws, address);
    std::string str = "signup2/" + new_username + "/" + password + "/" + purse + "/" + phoneNumber + "/" + address;
    return str;
}

std::string define_command(std::string command)
{
    std::stringstream ss(command);
    std::string order,word, str;
    std::getline (ss, order, ' ');

    if(order == "signin")
    {
        std::getline (ss, word, ' ');
        std::string username = word;
        std::getline (ss, word, ' ');
        std::string password = word;
        str = "signin/" + username + "/" + password;
        
    }
    else if(order == "signup")
    {
        std::getline (ss, word, ' ');
        str = "signup/" + word;
    }
    else
        str = "error";
    return str;
}

std::string show_list()
{
    std::cout << "Please choose one of the following commands:" << std::endl;
    std::cout << "signin <username> <password>" << std::endl;
    std::cout << "signup <username>" << std::endl;

    std::string str;
    std::getline(std::cin >> std::ws, str);
    return str;
}

int Client::connectServer(int port)
{
    int fd;
    struct sockaddr_in server_address;

    fd = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = inet_addr(data.getHostName().c_str());

    if (connect(fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    { // checking for errors
        printf("Error in connecting to server\n");
    }

    return fd;
}

void Client::build()
{
    std::cout << "Client is running..." << std::endl;
    int fd = connectServer(data.getPort());
    char buffer[1024] = {0};
    while (true)
    {
        std::string command;
        std::string str(buffer);
        
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;
        while (std::getline(ss, token, '/')){
            tokens.push_back(token);
        }
        
        //logged in list
        if(!tokens.empty() && (tokens[0] == ERR230 || isNumberBetween0And9(tokens[0])))
        {
            user_list();
        }

        //print error
        if (tokens.empty() || tokens[0] != ERR311)
        {
            command = show_list();
            command = define_command(command);
            if (command == "error" && str != ERR503)
            {
                std::cout << ERR503 << std::endl;
                continue;
            }
        }

        //get data for signup
        if (!tokens.empty() && tokens[0] == ERR311)
        {
            command = get_data_signup(tokens[1]);
        }
        send(fd, command.c_str(), command.size(), 0);
        memset(buffer, 0, sizeof(buffer));
        read(fd, buffer, 1024);
        std::cout << buffer << std::endl;
    }
}

int main(int argc, char const *argv[])
{
    readJson data;
    Client client;
    client.build();
    return 0;
}
