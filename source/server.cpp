#include "../library/server.hpp"

Server::Server(readJson data_) : data(data_) {}

bool checkDateFormat(const std::string& input) {
    std::stringstream ss(input);
    int day, month, year;
    char delim;
    if (!(ss >> day >> delim >> month >> delim >> year)) {
        return false;
    }
    // Check if all parts are integers
    if (ss.fail() || !ss.eof()) {
        return false;
    }
    // Check for valid date
    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900 || year > 2100) {
        return false;
    }
    //check the length of each part
    std::string str = std::to_string(day);
    if(str.size() != 2)
        return false;
    str = std::to_string(month);
    if(str.size() != 2)
        return false;
    str = std::to_string(year);
    if(str.size() != 4)
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
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    
    listen(server_fd, 4);

    return server_fd;
}
void Server::build()
{
    fd_set master_set, working_set;
    int server_fd;
    char buffer[1024] = {0};
    server_fd = setup_server(data.getPort());
    
    std::cout << "enter the date:\n";
    std::string input;
    std::getline(std::cin, input);
    std::stringstream ss(input);
    std::string word;
    ss >> word;
    if(word != "setTime")
    {
        std::cout << "Bad sequence of commands.\n";//error 503
        exit(EXIT_FAILURE);
    }
    std::string date;
    ss >> date;
    if (!checkDateFormat(date))
    {
        //error 401
        std::cout << "Invalid value!\n";
        exit(EXIT_FAILURE);
    }
    if(ss >> word)
    {
        std::cout << "Bad sequence of commands.\n";//error 503
        exit(EXIT_FAILURE);
    }
    
}
int main(int argc, char * argv[])
{
    readJson data;
    Server server(data);
    server.build();
    return 0;
}
