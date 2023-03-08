#include "../library/server.hpp"

Server::Server(readJson data_) : data(data_) {}

void Server::set_date(std::string day_, std::string month_, std::string year_)
{
    sys_date.day = day_;
    sys_date.month = month_;
    sys_date.year = year_;
}

void Server::pass_day(int id, int fd, std::istringstream& ss)
{
    std::string message;
    std::string value;
    std::getline(ss, value, '/'); //value

    //check if value is empty
    if (value.empty())
    {
        message = ERR503;
        message += "/" + std::to_string(id) + "/admin";
        std::cout << "User id: " << id << " tried to pass day." << std::endl;
        send(fd, message.c_str(), message.size(), 0);
        return;
    }

    //check if value is a number
    if (checkIsANumber(value, fd) == false)
    {
        message = ERR503;
        message += "/" + std::to_string(id) + "/admin";
        std::cout << "User id: " << id << " tried to pass day." << std::endl;
        send(fd, message.c_str(), message.size(), 0);
        return;
    }
    int day = std::stoi(value);

    //check if value is positive
    if (day <= 0)
    {
        message = ERR401;
        message += "/" + std::to_string(id) + "/admin";
        std::cout << "User id: " << id << " tried to pass day." << std::endl;
        send(fd, message.c_str(), message.size(), 0);
        return;
    }

    int day_ = std::stoi(sys_date.day);
    int month_ = std::stoi(sys_date.month);
    int year_ = std::stoi(sys_date.year);
    day_ += day;
    if (month_ == 1 || month_ == 3 || month_ == 5 || month_ == 7 || month_ == 8 || month_ == 10 || month_ == 12)
    {
        if (day_ > 31)
        {
            month_++;
            day_ -= 31;
        }
    }
    else if (month_ == 4 || month_ == 6 || month_ == 9 || month_ == 11)
    {
        if (day_ > 30)
        {
            month_++;
            day_ -= 30;
        }
    }
    else if (month_ == 2)
    {
        if (year_ % 4 == 0)
        {
            if (day_ > 29)
            {
                month_++;
                day_ -= 29;
            }
        }
        else
        {
            if (day_ > 28)
            {
                month_++;
                day_ -= 28;
            }
        }
    }
    if (month_ > 12)
    {
        year_++;
        month_ -= 12;
    }
    sys_date.day = std::to_string(day_);
    sys_date.month = std::to_string(month_);
    sys_date.year = std::to_string(year_);
    message = ERR312;
    message += "/" + std::to_string(id) + "/admin";
    std::cout << "Admin id: " << id << " passed " << day << " days." << std::endl;
    std::cout << "Date: " << sys_date.day << "-" << sys_date.month << "-" << sys_date.year << std::endl;
    send(fd, message.c_str(), message.size(), 0);
    return;
}

void Server::edit_information(int id, int fd, std::istringstream& ss)
{
    std::string message;
    for (int i = 0; i < data.users.size(); i++)
    {
        if (data.users[i]->getid() == id)
        {
            std::string password, phone, address;
            std::getline(ss, password, '/'); //password
            std::getline(ss, phone, '/'); //phone
            std::getline(ss, address, '/'); //address
            if (data.users[i]->getpassword() == password || data.users[i]->getaddress() == address || data.users[i]->getphone() == phone || checkIsANumber(phone, fd) == false)
            {
                message = ERR503;
                message += "/" + std::to_string(data.users[i]->getid()) + "/user";
                std::cout << "User id: " << id << " tried to edit information." << std::endl;
                send(fd, message.c_str(), message.size(), 0);
                return;
            }
            data.users[i]->setpassword(password);
            data.users[i]->setaddress(address);
            data.users[i]->setphoneNumber(phone);
            message = ERR312;
            message += "/" + std::to_string(data.users[i]->getid()) + "/user";
            std::cout << "User id: " << id << " edited information." << std::endl;
            send(fd, message.c_str(), message.size(), 0);
            return;
        }
    }
    for (int i = 0; i < data.admins.size(); i++)
    {
        if (data.admins[i]->getid() == id)
        {
            std::string password;
            std::getline(ss, password, '/'); //password
            if (data.admins[i]->getpassword() == password)
            {
                message = ERR503;
                message += "/" + std::to_string(data.admins[i]->getid()) + "/admin";
                std::cout << "Admin id: " << id << " tried to edit information." << std::endl;
                send(fd, message.c_str(), message.size(), 0);
                return;
            }
            data.admins[i]->setpassword(password);
            message = ERR312;
            message += "/" + std::to_string(data.admins[i]->getid()) + "/admin";
            std::cout << "Admin id: " << id << " edited information." << std::endl;
            send(fd, message.c_str(), message.size(), 0);
            return;
        }
    }
    return;
}

void Server::leave_room(int id, int fd, std::istringstream& ss)
{
    std::string message, user_admin;
    std::string value, roomOrCapacity;
    std::getline(ss, roomOrCapacity, '/'); //roomOrCapacity
    std::getline(ss, value, '/'); //room_number

    if (check_if_is_admin(id) == true)
        user_admin = "admin";
    else
        user_admin = "user";

    //check if there is empty field
    if (value == "")
    {
        message = ERR503;
        message += "/" + std::to_string(id) + "/" + user_admin;
        std::cout << "User id: " << id << " tried to leave room." << std::endl;
        send(fd, message.c_str(), message.size(), 0);
        return;
    }

    //check if value is number
    if (checkIsANumber(value, fd) == false)
    {
        message = ERR503;
        message += "/" + std::to_string(id) + "/" + user_admin;
        std::cout << "User id: " << id << " tried to leave room." << std::endl;
        send(fd, message.c_str(), message.size(), 0);
        return;
    }

    if(roomOrCapacity == "room")
    {
        for (int i = 0; i < data.rooms.size(); i++)
        {
            if (data.rooms[i]->getnum() == value)  //check if room exist
            {
                std::vector<userInRoom> usersExists = data.rooms[i]->getusers();
                int size = usersExists.size();
                for (int j = 0; j < size; j++)
                {
                    if (usersExists[j].id == id) //check if user exist in room
                    {
                        usersExists.erase(usersExists.begin() + j);
                        data.rooms[i]->set_capacity(data.rooms[i]->getcapacity() + 1);
                        if(data.rooms[i]->getcapacity() == 1)
                            data.rooms[i]->set_capacity(0);
                        //set new userexist vector in room
                        data.rooms[i]->set_userInRooms(usersExists);
                        message = ERR413;
                        message += "/" + std::to_string(id) + "/" + user_admin;
                        std::cout << "User id: " << id << " left room." << std::endl;
                        send(fd, message.c_str(), message.size(), 0);
                        return;
                    }
                }

                //user not exist in room
                message = ERR102;
                message += "/" + std::to_string(id) + "/" + user_admin;
                std::cout << "User id: " << id << " tried to leave room." << std::endl;
                send(fd, message.c_str(), message.size(), 0);
                return;
            }
        }

        //room not exist
        message = ERR503;
        message += "/" + std::to_string(id) + "/" + user_admin;
        std::cout << "User id: " << id << " tried to leave room." << std::endl;
        send(fd, message.c_str(), message.size(), 0);
        return;
    }
}

bool Server::check_room_exist(std::string room_number)
{
    for (int i = 0; i < data.rooms.size(); i++)
    {
        if (data.rooms[i]->getnum() == room_number)
            return true;
    }
    return false;
}

void Server::edit_rooms(int id, int fd, std::istringstream& ss)
{
    std::string command, message;
    std::getline(ss, command, '/'); //command
    if(command == "add")
    {
        std::string RoomNum, MaxCapacity, Price;
        std::getline(ss, RoomNum, '/'); //RoomNum
        std::getline(ss, MaxCapacity, '/'); //MaxCapacity
        std::getline(ss, Price, '/'); //Price

        //check if there is empty field
        if (RoomNum == "" || MaxCapacity == "" || Price == "")
        {
            message = ERR503;
            message += "/" + std::to_string(id) + "/admin";
            std::cout << "Admin id: " << id << " tried to add room." << std::endl;
            send(fd, message.c_str(), message.size(), 0);
            return;
        }

        //check if the fields are numbers
        if (checkIsANumber(RoomNum, fd) == false || checkIsANumber(MaxCapacity, fd) == false || checkIsANumber(Price, fd) == false)
        {
            message = ERR503;
            message += "/" + std::to_string(id) + "/admin";
            std::cout << "Admin id: " << id << " tried to add room." << std::endl;
            send(fd, message.c_str(), message.size(), 0);
            return;
        }

        //check if room numbeer is already exist
        for(int i = 0; i < data.rooms.size(); i++)
        {
            if (data.rooms[i]->getnum() == RoomNum)
            {
                message = ERR111;
                message += "/" + std::to_string(id) + "/admin";
                std::cout << "Admin id: " << id << " tried to add room." << std::endl;
                send(fd, message.c_str(), message.size(), 0);
                return;
            }
        }
        std::vector<userInRoom> newusers;
        data.rooms.push_back(new Rooms(RoomNum, 0, stoi(Price), stoi(MaxCapacity), 0, newusers));
        
        //send message to client that room added
        message = ERR104;
        message += "/" + std::to_string(id) + "/admin";
        std::cout << "Admin id: " << id << " added room." << std::endl;
        send(fd, message.c_str(), message.size(), 0);
        return; 
    }
    else if(command == "modify")
    {
        std::string RoomNum, newMaxCapacity, newPrice;
        std::getline(ss, RoomNum, '/'); //RoomNum
        std::getline(ss, newMaxCapacity, '/'); //newMaxCapacity
        std::getline(ss, newPrice, '/'); //newPrice

        //check if there is empty field
        if (RoomNum == "" || newMaxCapacity == "" || newPrice == "")
        {
            message = ERR503;
            message += "/" + std::to_string(id) + "/admin";
            std::cout << "Admin id: " << id << " tried to modify room." << std::endl;
            send(fd, message.c_str(), message.size(), 0);
            return;
        }

        //check if the fields are numbers
        if (checkIsANumber(RoomNum, fd) == false || checkIsANumber(newMaxCapacity, fd) == false || checkIsANumber(newPrice, fd) == false)
        {
            message = ERR503;
            message += "/" + std::to_string(id) + "/admin";
            std::cout << "Admin id: " << id << " tried to modify room." << std::endl;
            send(fd, message.c_str(), message.size(), 0);
            return;
        }

        //check if room number doesn't exist
        if(check_room_exist(RoomNum) == false)
        {
            message = ERR101;
            message += "/" + std::to_string(id) + "/admin";
            std::cout << "Admin id: " << id << " tried to modify room." << std::endl;
            send(fd, message.c_str(), message.size(), 0);
            return;
        }

        //check if the room is full
        for(int i = 0; i < data.rooms.size(); i++)
        {
            if (data.rooms[i]->getnum() == RoomNum)
            {
                if (data.rooms[i]->getstatus() == 1)
                {
                    message = ERR109;
                    message += "/" + std::to_string(id) + "/admin";
                    std::cout << "Admin id: " << id << " tried to modify room." << std::endl;
                    send(fd, message.c_str(), message.size(), 0);
                    return;
                }
            }
        }

        //check if new values are not same as old values
        for(int i = 0; i < data.rooms.size(); i++)
        {
            if (data.rooms[i]->getnum() == RoomNum)
            {
                if (data.rooms[i]->getprice() == stoi(newPrice) || data.rooms[i]->getmax_capacity() == stoi(newMaxCapacity))
                {
                    message = ERR503;
                    message += "/" + std::to_string(id) + "/admin";
                    std::cout << "Admin id: " << id << " tried to modify room." << std::endl;
                    send(fd, message.c_str(), message.size(), 0);
                    return;
                }
            }
        }
        
        //modify the room
        for(int i = 0; i < data.rooms.size(); i++)
        {
            if (data.rooms[i]->getnum() == RoomNum)
            {
                data.rooms[i]->set_price(stoi(newPrice));
                data.rooms[i]->set_maxcap(stoi(newMaxCapacity));
                break;
            }
        }
        message = ERR105;
        message += "/" + std::to_string(id) + "/admin";
        std::cout << "Admin id: " << id << " modified room." << std::endl;
        send(fd, message.c_str(), message.size(), 0);
        return;
    }
    else if(command == "remove")
    {
        std::string RoomNum;
        std::getline(ss, RoomNum, '/'); //RoomNum

        //check if there is empty field
        if (RoomNum == "")
        {
            message = ERR503;
            message += "/" + std::to_string(id) + "/admin";
            std::cout << "Admin id: " << id << " tried to remove room." << std::endl;
            send(fd, message.c_str(), message.size(), 0);
            return;
        }

        //check if the fields are numbers
        if (checkIsANumber(RoomNum, fd) == false)
        {
            message = ERR503;
            message += "/" + std::to_string(id) + "/admin";
            std::cout << "Admin id: " << id << " tried to remove room." << std::endl;
            send(fd, message.c_str(), message.size(), 0);
            return;
        }

        //check if room number doesn't exist
        if(check_room_exist(RoomNum) == false)
        {
            message = ERR101;
            message += "/" + std::to_string(id) + "/admin";
            std::cout << "Admin id: " << id << " tried to remove room." << std::endl;
            send(fd, message.c_str(), message.size(), 0);
            return;
        }

        //check if the room is full
        for(int i = 0; i < data.rooms.size(); i++)
        {
            if (data.rooms[i]->getnum() == RoomNum)
            {
                if (data.rooms[i]->getstatus() == 1)
                {
                    message = ERR109;
                    message += "/" + std::to_string(id) + "/admin";
                    std::cout << "Admin id: " << id << " tried to remove room." << std::endl;
                    send(fd, message.c_str(), message.size(), 0);
                    return;
                }
            }
        }

        //remove the room
        for(int i = 0; i < data.rooms.size(); i++)
        {
            if (data.rooms[i]->getnum() == RoomNum)
            {
                data.rooms.erase(data.rooms.begin() + i);
                break;
            }
        }
        message = ERR106;
        message += "/" + std::to_string(id) + "/admin";
        std::cout << "Admin id: " << id << " removed room." << std::endl;
        send(fd, message.c_str(), message.size(), 0);
        return;
    }
    else
    {
        std::cout << "Admin id: " << id << " tried to edit rooms." << std::endl;
        send(fd, ERR503, strlen(ERR503), 0);
    }
}

void Server::logout(int id, int fd)
{
    for (int i = 0; i < loggedInIds.size(); i++)
    {
        if (loggedInIds[i] == id)
        {
            loggedInIds.erase(loggedInIds.begin() + i);
            std::cout << "User id: " << id << " logged out." << std::endl;
            send(fd, ERR201, strlen(ERR201), 0);
            break;
        }
    }
}

std::string Server::rooms_info_gathering(int id)///
{
    std::stringstream ss;
    bool is_admin = check_if_is_admin(id);
    ss << "###########################" << std::endl;
    ss << "rooms info: \n";
    for (int i = 0; i < data.rooms.size(); i++)
    {
        ss << "number: " << data.rooms[i]->getnum() << std::endl;
        ss << "status: " << data.rooms[i]->getstatus() << std::endl;
        ss << "price: " << data.rooms[i]->getprice() << std::endl;
        ss << "maxCapacity: " << data.rooms[i]->getmax_capacity() << std::endl;
        ss << "capacity: " << data.rooms[i]->getcapacity() << std::endl;
        if (is_admin)
        {
            ss << "------------------------" << std::endl;
            ss << "users in this room: " << std::endl;
            bool is_empty = true;
            for (int j = 0; j < data.rooms[i]->getusers().size(); j++)
            {
                is_empty = false;
                if(j) ss << "------------------------" << std::endl;
                ss << "user number " << j + 1 << ": " << std::endl;
                ss << "id: " << data.rooms[i]->getusers()[j].id << std::endl;
                ss << "number of reserved beds: " << data.rooms[i]->getusers()[j].numOfBeds << std::endl;
                ss << "from date: " << data.rooms[i]->getusers()[j].reserveDate << std::endl;
                ss << "to date: " << data.rooms[i]->getusers()[j].checkoutDate << std::endl;
            }
            if (is_empty)
                ss << "this room is empty." << std::endl;
        }
        ss << "###########################" << std::endl;
    }
    ss << "/" << id;
    if (is_admin)
        ss << "/admin";
    else
        ss << "/user";
    std::string room_info;
    room_info = ss.str();
    return room_info;
}

std::string Server::user_info_gathering(int id)
{
    std::stringstream ss;
    ss << "###########################" << std::endl;
    ss << "all users info:" << std::endl; 
    for(int i = 0; i < data.users.size(); i++)
    {

        ss << "id: " << std::to_string(data.users[i]->getid()) << std::endl;
        ss << "name: " << data.users[i]->getname() << std::endl;
        ss << "purse: " << data.users[i]->getpurse() << std::endl;
        ss << "phoneNumber: " << data.users[i]->getphone() << std::endl;
        ss << "address: " << data.users[i]->getaddress() << std::endl;
        ss << "###########################" << std::endl;
    }
    ss << "/" << id;
    std::string info;
    info = ss.str();
    return info;

}

bool Server::check_if_is_admin(int id)
{
    for (int i = 0; i < data.admins.size(); i++)
    {
        if (id == data.admins[i]->getid())
            return true;
    }
    return false;
}

std::string Server::get_info(int id)
{
    std::stringstream ss;
    ss << "###########################" << std::endl;
    for (int i = 0; i < data.users.size(); i++)
    {
        if (id == data.users[i]->getid())
        {
            ss << "id: " << std::to_string(data.users[i]->getid()) << std::endl;
            ss << "name: " << data.users[i]->getname() << std::endl;
            ss << "password: " << data.users[i]->getpassword() << std::endl;
            ss << "purse: " << data.users[i]->getpurse() << std::endl;
            ss << "phoneNumber: " << data.users[i]->getphone() << std::endl;
            ss << "address: " << data.users[i]->getaddress() << std::endl;
            ss << "###########################" << std::endl;
            ss << "/" << id <<"/user";
            break;
        }
    }
    for (int i = 0; i < data.admins.size(); i++)
    {
        if (id == data.admins[i]->getid())
        {
            ss << "id: " << std::to_string(data.users[i]->getid()) << std::endl;
            ss << "name: " << data.users[i]->getname() << std::endl;
            ss << "password: " << data.users[i]->getpassword() << std::endl;
            ss << "###########################" << std::endl;
            ss << "/" << id <<"/admin";
            break;
        }
    }
    ss << "/" << id;
    std::string info;
    info = ss.str();
    return info;
}

void Server::action_to_be_done(int choice, int id, int fd, std::istringstream& ss)
{
    switch(choice)
    {
        case 1: //View user information
        {
            std::string userORadmin_info;
            userORadmin_info = get_info(id);
            send(fd, userORadmin_info.c_str(), userORadmin_info.size(), 0);
            break;
        }
        case 2: //View all users
        {
            std::string info;
            if (check_if_is_admin(id))
            {
                info = user_info_gathering(id);
            }
            else
            {
                std::stringstream ss;
                ss << ERR403 << "/" << id << "/user";
                info = ss.str();
            }
            send(fd, info.c_str(), info.size(), 0);
            break;
        }
        case 3: //View rooms information
        {
            std::string rooms_info;
            rooms_info = rooms_info_gathering(id);
            send(fd, rooms_info.c_str(), rooms_info.size(), 0);
            break;
        }
        case 4:
            //Booking
            break;
        case 5:
            //Canceling
            break;
        case 6:
            pass_day(id, fd, ss);
            break;
        case 7:
            edit_information(id, fd, ss);
            break;
        case 8:
            leave_room(id, fd, ss);
            break;
        case 9:
            edit_rooms(id, fd, ss);
            break;
        case 0:
            logout(id, fd);
            break;
    }
}

void Server::signin(std::string username, std::string password, int fd)
{
    std::string message;
    for (int i = 0; i < data.users.size(); i++)
    {
        if (data.users[i]->getname() == username && data.users[i]->getpassword() == password)
        {
            message = ERR230;
            message += "/" + std::to_string(data.users[i]->getid()) + "/user" ;
            loggedInIds.push_back(data.users[i]->getid());
            send(fd, message.c_str(), message.size(), 0);
            return;
        }
    }
    
    for (int i = 0; i < data.admins.size(); i++)
    {
        if (data.admins[i]->getname() == username && data.admins[i]->getpassword() == password)
        {
            message = ERR230;
            message += "/" + std::to_string(data.admins[i]->getid()) + "/admin";
            loggedInIds.push_back(data.admins[i]->getid());
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
    data.users.push_back(new User(data.users.size()+ data.admins.size(), username, password, purse, phoneNumber, address));
    
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
    
    //check if the admin already exists
    for (int i = 0; i < data.admins.size();i++)
    {
        std::string username = data.admins[i]->getname();
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
    std::istringstream ss(command);
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
    else if(order == "menu")
    {
        std::string command_num, id;
        std::getline(ss, command_num, '/');
        std::getline(ss, id, '/');
        action_to_be_done(stoi(command_num), stoi(id), fd, ss);
    }
}

bool Server::checkDateFormat(const std::string& input) {
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
    set_date(day, month, year);
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
