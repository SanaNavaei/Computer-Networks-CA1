#include "../library/server.hpp"

Server::Server(readJson data_) : data(data_) {}

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
int main(int argc, char * argv[])
{
    readJson data;
    Server server(data);
    if(!server.build())
        std::cout << "error in building" << std::endl;
    return 0;
}
