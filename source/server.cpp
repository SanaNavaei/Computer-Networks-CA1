#include "../library/server.hpp"

Server::Server(readJson data_) : data(data_) {}

void Server::signin(std::string username, std::string password, int fd)
{
    std::string message;
    for (int i = 0; i < data.users.size(); i++)
    {
        if (data.users[i]->getname() == username && data.users[i]->getpassword() == password)
        {
            message = ERR230;
            send(fd, message.c_str(), message.size(), 0);
            return;
        }
    }
    message = ERR430;
    send(fd, message.c_str(), message.size(), 0);
    return;
}

bool Server::checkIsANumber(std::string input, int fd)
{
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] < '0' || input[i] > '9')
        {
            return false;
        }
    }
    return true;
}

void Server::signup(std::string username, std::string password, std::string purse, std::string phoneNumber, 
                    std::string address, int fd)
{
    if(checkIsANumber(purse, fd) == false)
    {
        std::string message = ERR503;
        send(fd, message.c_str(), message.size(), 0);
        return;
    }
    else if(checkIsANumber(phoneNumber, fd) == false)
    {
        std::string message = ERR503;
        send(fd, message.c_str(), message.size(), 0);
        return;
    }
    data.users.push_back(new User(data.users.size(), username, password, purse, phoneNumber, address));
    
    std::string message = ERR231;
    send(fd, message.c_str(), message.size(), 0);
}

void Server::checkusername(std::string name, int fd)
{
    std::string message;

    //check if the user already exists
    for (int i = 0; i < data.users.size();i++)
    {
        std::string username = data.users[i]->getname();
        if (username == name)
        {
            message =  ERR451;
            send(fd, message.c_str(), message.size(), 0);
            return;
        }
    }
    std::stringstream ss;
    ss << ERR311 << "/" << name;
    message = ss.str();
    send(fd, message.c_str(), message.size(), 0);
}

void Server::checkCommand(char buff[], int fd)
{
    std::string command(buff);
    std::stringstream ss(command);
    std::string order, word, str;
    std::getline(ss, order, '/');
    if (order == "signup")
    {
        std::getline(ss, word, '/');
        if (word == "") 
        {
            std::string error =  ERR430;
            send(fd, error.c_str(), error.size(), 0);
        }
        checkusername(word,fd);
    }
    else if (order == "signin")
    {
        std::string username, password;
        std::getline (ss, username, '/');
        std::getline (ss, password, '/');
        if (username == "" || password == "")
        {
            std::string error =  ERR430;
            send(fd, error.c_str(), error.size(), 0);
        }
        else 
            signin(username, password, fd);
    }
    else if(order == "signup2")
    {
        std::string username, password, purse, phoneNumber, address;
        std::getline (ss, username, '/');
        std::getline (ss, password, '/');
        std::getline (ss, purse, '/');
        std::getline (ss, phoneNumber, '/');
        std::getline (ss, address, '/');

        signup(username, password, purse, phoneNumber, address, fd);
    }
}

bool checkDateFormat(const std::string& input) {
    std::stringstream ss(input);
    std::string day, month, year;
    
    std::getline (ss, day, DELIM);
    std::getline (ss, month, DELIM);
    std::getline (ss,year, DELIM);
    
    // Check if all parts are integers
    if (ss.fail() || !ss.eof()) {
        return false;
    }
    // Check for valid date
    if (stoi(day) < 1 || stoi(day) > 31 || stoi(month) < 1 || stoi(month) > 12 || stoi(year) < 1900 || stoi(year) > 2100) {
        return false;
    }
    //check the length of each part
    if(day.size() != 2)
        return false;
    if(month.size() != 2)
        return false;
    if(year.size() != 4)
        return false;
    return true;
}

int Server::setup_server(int port)
{
    struct sockaddr_in address;
    int server_fd;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    
    //check if there is more than one server running on the same port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cout << "There is a server running on this port." << std::endl;
        exit(EXIT_FAILURE);
    }

    listen(server_fd, MAX_CLIENTS);

    return server_fd;
}

int acceptClient(int server_fd) 
{
    int client_fd;
    struct sockaddr_in client_address;
    int address_len = sizeof(client_address);
    client_fd = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t*) &address_len);
    return client_fd;
}

void Server::build()
{
    fd_set master_set, working_set;
    int server_fd;
    char buffer[1024] = {0};
    server_fd = setup_server(data.getPort());
    if (server_fd < 0)
    {
        std::cout << "error in building." << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "server is running." << std::endl;
    
    std::cout << "enter the date:\n";
    std::string input;
    std::getline(std::cin, input);
    std::stringstream ss(input);
    std::string word;
    ss >> word;
    if(word != "setTime")
    {
        std::cout << ERR503 << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string date;
    ss >> date;
    if (!checkDateFormat(date))
    {
        std::cout << ERR401 << std::endl;
        exit(EXIT_FAILURE);
    }
    if(ss >> word)
    {
        std::cout << ERR503 << std::endl;
        exit(EXIT_FAILURE);
    }
    
    FD_ZERO(&master_set);
    FD_ZERO(&working_set);
    int max_sd = server_fd;
    FD_SET(server_fd, &master_set);
    
    while (true)
    {
        working_set = master_set;
        select(max_sd + 1, &working_set, NULL, NULL, NULL);

        for (int i = 0; i <= max_sd; i++)
        {
            if (FD_ISSET(i, &working_set))
            {
                if (i == server_fd) //new client
                {
                    int client_fd = acceptClient(server_fd);
                    FD_SET(client_fd, &master_set);
                    if (client_fd > max_sd)
                        max_sd = client_fd;
                }
                else //client is sending a message
                {
                    memset(buffer, 0, 1024);
                    int bytes_recieved = recv(i, buffer, 1024, 0);
                    if(bytes_recieved == 0)
                    {
                        std::cout << "client disconnected." << std::endl;
                        close(i);
                        FD_CLR(i, &master_set);
                        continue;
                    }
                    checkCommand(buffer,i);
                }
            }
        }
    }
}

int main(int argc, char * argv[])
{
    readJson data;
    Server server(data);
    server.build();
    return 0;
}
