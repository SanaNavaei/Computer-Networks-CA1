#include "../library/server.hpp"

std::stringstream log_m;  

void logMessage(std::string message) {
    std::ofstream logfile;
    logfile.open("logfile.txt", std::ios_base::app); // open the file in append mode
    if (logfile.is_open()) {
        auto time_now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string timestamp = std::ctime(&time_now); // get the current time as a string
        logfile << timestamp.substr(0, timestamp.length()-1) << " - " << message << std::endl;
        logfile.close();
    } else {
        std::cerr << "Failed to open log file" << std::endl;
    }
}

Server::Server(readJson data_) : data(data_) {}

void Server::set_date(std::string day_, std::string month_, std::string year_)
{
    sys_date.day = day_;
    sys_date.month = month_;
    sys_date.year = year_;
}

bool Server::compare_date(std::string date)
{
    std::string day = date.substr(0, 2);
    std::string month = date.substr(3, 2);
    std::string year = date.substr(6, 4);
    
    //comapre year
    if (std::stoi(year) > std::stoi(sys_date.year))
        return true;
    else if (std::stoi(year) < std::stoi(sys_date.year))
        return false;
    
    //compare month
    if (std::stoi(month) > std::stoi(sys_date.month))
        return true;
    else if (std::stoi(month) < std::stoi(sys_date.month))
        return false;

    //compare day
    if (std::stoi(day) > std::stoi(sys_date.day))
        return true;
    else if (std::stoi(day) < std::stoi(sys_date.day))
        return false;
    
    return false;
}

void Server::handle_pass_day()
{
    //check all rooms reservation to see if they are expired
    for (auto& room : data.rooms)
    {
        std::vector<int> deleted_index = {};
        std::vector<userInRoom> users = room->getusers();
        for(int i = 0; i < users.size(); i++)
        {
            //if reservation is expired
            if (compare_date(users[i].checkoutDate) == false)
            {
                room->set_capacity(room->getcapacity() + users[i].numOfBeds);
                if(room->getstatus() == 1)
                    room->set_status(0);
                deleted_index.push_back(i);
                std::string jsondata = "{\"id\":" + std::to_string(users[i].id) + ",\"number\":\"" + room->getnum() + 
                                        "\",\"capacity\":" + std::to_string(room->getcapacity()) + 
                                        ",\"status\":" + std::to_string(room->getstatus()) + "}";
                data.write_leaveroom(jsondata);
            }
        }
        //delete expired reservation
        for (int i = 0; i < deleted_index.size(); i++)
            users.erase(users.begin() + deleted_index[i]);
        room->set_userInRooms(users);  
    }
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
    if(day_ < 10)
        sys_date.day = "0" + std::to_string(day_);
    else
        sys_date.day = std::to_string(day_);
    if(month_ < 10)
        sys_date.month = "0" + std::to_string(month_);
    else
        sys_date.month = std::to_string(month_);
    sys_date.day = std::to_string(day_);
    sys_date.month = std::to_string(month_);
    sys_date.year = std::to_string(year_);
    message = ERR312;
    message += "/" + std::to_string(id) + "/admin";
    std::cout << "Admin id: " << id << " passed " << day << " days." << std::endl;
    std::cout << "Date: " << sys_date.day << "-" << sys_date.month << "-" << sys_date.year << std::endl;
    handle_pass_day();
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
            
            std::string jsondata = "{\"id\":" + std::to_string(id) + ",\"password\":\"" + password + "\",\"phoneNumber\":\"" + phone + "\",\"address\":\"" + address + "\",\"admin\":\"false\"}";
            send(fd, message.c_str(), message.size(), 0);
            data.write_editinfo(jsondata);
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
            std::string jsondata = "{\"id\":" + std::to_string(id) + ",\"password\":\"" + password + "\",\"admin\":\"true\"}";
            send(fd, message.c_str(), message.size(), 0);
            data.write_editinfo(jsondata);
            return;
        }
    }
    return;
}

void Server::user_leave_room(int id, int fd, std::istringstream& ss)
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
                        //error if user leave room before reserve date
                        if(compare_date(usersExists[j].reserveDate) == true)
                        {
                            message = ERR503;
                            message += "/" + std::to_string(id) + "/" + user_admin;
                            std::cout << "User id: " << id << " tried to leave room." << std::endl;
                            send(fd, message.c_str(), message.size(), 0);
                            return;
                        }
                        if (compare_date(usersExists[j].reserveDate) == false && compare_date(usersExists[j].checkoutDate) == true)
                        {
                            data.rooms[i]->set_capacity(data.rooms[i]->getcapacity() + usersExists[j].numOfBeds);
                            if(data.rooms[i]->getstatus() == 1)
                                data.rooms[i]->set_status(0);
                        }
                        usersExists.erase(usersExists.begin() + j);
                        //set new userexist vector in room
                        data.rooms[i]->set_userInRooms(usersExists);
                        std:: string jsondata = "{\"id\":" + std::to_string(id) + ",\"number\":\"" + value + 
                                                "\",\"capacity\":" + std::to_string(data.rooms[i]->getcapacity()) + 
                                                ",\"status\":" + std::to_string(data.rooms[i]->getstatus()) + ",\"admin\":\"false\"}";  
                        message = ERR413;
                        message += "/" + std::to_string(id) + "/" + user_admin;
                        std::cout << "User id: " << id << " left room." << std::endl;
                        send(fd, message.c_str(), message.size(), 0);
                        data.write_leaveroom(jsondata);
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

void Server::admin_leave_room(int id, int fd, std::istringstream& ss)
{
    std::string room_number, capacity;
    std::string message;
    std::getline(ss, room_number, '/'); //room_number
    std::getline(ss, capacity, '/'); //capacity

    //check if there is empty field
    if (room_number == "" || capacity == "")
    {
        message = ERR503;
        message += "/" + std::to_string(id) + "/admin";
        std::cout << "Admin id: " << id << " tried to leave room." << std::endl;
        send(fd, message.c_str(), message.size(), 0);
        return;
    }

    //check if value is number
    if (checkIsANumber(room_number, fd) == false || checkIsANumber(capacity, fd) == false)
    {
        message = ERR503;
        message += "/" + std::to_string(id) + "/admin";
        std::cout << "Admin id: " << id << " tried to leave room." << std::endl;
        send(fd, message.c_str(), message.size(), 0);
        return;
    }

    for (int i = 0; i < data.rooms.size(); i++)
    {
        if (data.rooms[i]->getnum() == room_number)  //check if room exist
        {
            if(capacity == "0")
            {
                //In this part, we remove the users from chosen room and set the room status to 0 and capacity to max capacity
                data.rooms[i]->set_capacity(data.rooms[i]->getmax_capacity());
                data.rooms[i]->set_status(0);
                data.rooms[i]->set_userInRooms({});
                std:: string jsondata = "{\"id\":" + std::to_string(id) + ",\"number\":\"" + room_number + 
                                    "\",\"capacity\":" + std::to_string(data.rooms[i]->getcapacity()) + 
                                    ",\"status\":" + std::to_string(data.rooms[i]->getstatus()) + ",\"users\":" + "[]}";
                message = ERR413;
                message += "/" + std::to_string(id) + "/admin";
                std::cout << "Admin id: " << id << " made room empty." << std::endl;
                send(fd, message.c_str(), message.size(), 0);
                data.write_capacity(jsondata);
                return;
            }

            //check new capacity is bigger than current capacity
            else if (std::stoi(capacity) > data.rooms[i]->getcapacity())
            {
                message = ERR412;
                message += "/" + std::to_string(id) + "/admin";
                std::cout << "Admin id: " << id << " tried to empty room." << std::endl;
                send(fd, message.c_str(), message.size(), 0);
                return;
            }

            //error if new capacity is smaller than current capacity and it is not 0
            else
            {
                message = ERR401;
                message += "/" + std::to_string(id) + "/admin";
                std::cout << "Admin id: " << id << " tried to empty room." << std::endl;
                send(fd, message.c_str(), message.size(), 0);
                return;
            }
        }
    }

    //room not exist
    message = ERR101;
    message += "/" + std::to_string(id) + "/admin";
    std::cout << "Admin id: " << id << " tried to leave room." << std::endl;
    send(fd, message.c_str(), message.size(), 0);
    return;
}

void Server::leave_room(int id, int fd, std::istringstream& ss)
{
    if(check_if_is_admin(id))
        admin_leave_room(id, fd, ss);
    else 
        user_leave_room(id, fd, ss);

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
            log_m.str("");
            log_m << "the admin with id " << id << " tried to add room but results in ERR503." << std::endl;
            logMessage(log_m.str());
            send(fd, message.c_str(), message.size(), 0);
            return;
        }

        //check if the fields are numbers
        if (checkIsANumber(RoomNum, fd) == false || checkIsANumber(MaxCapacity, fd) == false || checkIsANumber(Price, fd) == false)
        {
            message = ERR503;
            message += "/" + std::to_string(id) + "/admin";
            std::cout << "Admin id: " << id << " tried to add room." << std::endl;
            log_m.str("");
            log_m << "the admin with the id " << id << " tried to add a room but results in ERR503." << std::endl;
            logMessage(log_m.str());
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
                log_m.str("");
                log_m << "the admin with the id " << id << " tried to add a room with number: " << RoomNum 
                      << "max capacity: " << MaxCapacity << "price: " << Price << " but results in ERR111." << std::endl;
                logMessage(log_m.str());
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
        std::string jsondata = "{\"number\":\"" + RoomNum + "\",\"capacity\":" + MaxCapacity + 
                                ",\"maxCapacity\":" + MaxCapacity + ",\"price\":" + Price + ",\"status\":0,\"users\":[]}";
        log_m.str("");
        log_m << "the admin with the id " << id << " added a room with number: " << RoomNum 
              << "max capacity: " << MaxCapacity << "price: " << Price << " successfully." << std::endl;
        logMessage(log_m.str());
        send(fd, message.c_str(), message.size(), 0);
        data.write_addroom(jsondata);
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
            log_m.str("");
            log_m << "the admin with id " << id << " tried to modify room but results in ERR503." << std::endl;
            logMessage(log_m.str());
            send(fd, message.c_str(), message.size(), 0);
            return;
        }

        //check if the fields are numbers
        if (checkIsANumber(RoomNum, fd) == false || checkIsANumber(newMaxCapacity, fd) == false || checkIsANumber(newPrice, fd) == false)
        {
            message = ERR503;
            message += "/" + std::to_string(id) + "/admin";
            std::cout << "Admin id: " << id << " tried to modify room." << std::endl;
            log_m.str("");
            log_m << "the admin with id " << id << " tried to modify room but results in ERR503." << std::endl;
            logMessage(log_m.str());
            send(fd, message.c_str(), message.size(), 0);
            return;
        }

        //check if room number doesn't exist
        if(check_room_exist(RoomNum) == false)
        {
            message = ERR101;
            message += "/" + std::to_string(id) + "/admin";
            std::cout << "Admin id: " << id << " tried to modify room." << std::endl;
            log_m.str("");
            log_m << "the admin with the id " << id << " tried to modify a room with number: " << RoomNum 
                  << "new max capacity: " << newMaxCapacity << "new price: " << newPrice << " but results in ERR101." << std::endl;
            logMessage(log_m.str());
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
                    log_m.str("");
                    log_m << "the admin with the id " << id << " tried to modify a room with number: " << RoomNum 
                          << "new max capacity: " << newMaxCapacity << "new price: " << newPrice << " but results in ERR109." << std::endl;
                    logMessage(log_m.str());
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
                    log_m.str("");
                    log_m << "the admin with the id " << id << " tried to modify a room with number: " << RoomNum 
                          << "new max capacity: " << newMaxCapacity << "new price: " << newPrice << " but results in ERR503." << std::endl;
                    logMessage(log_m.str());
                    send(fd, message.c_str(), message.size(), 0);
                    return;
                }
            }
        }
        
        //modify the room
        int newCapacity, status = 0;
        for(int i = 0; i < data.rooms.size(); i++)
        {
            if (data.rooms[i]->getnum() == RoomNum)
            {
                int numOfUSers = data.rooms[i]->getmax_capacity() - data.rooms[i]->getcapacity();
                data.rooms[i]->set_price(stoi(newPrice));
                data.rooms[i]->set_maxcap(stoi(newMaxCapacity));
                newCapacity = stoi(newMaxCapacity) - numOfUSers;
                data.rooms[i]->set_capacity(newCapacity);
                if(newCapacity == 0)
                {
                    data.rooms[i]->set_status(1);
                    status = 1;
                }
                break;
            }
        }
        message = ERR105;
        message += "/" + std::to_string(id) + "/admin";
        std::cout << "Admin id: " << id << " modified room." << std::endl;
        std::string jsondata = "{\"number\":\"" + RoomNum + "\",\"capacity\":" + std::to_string(newCapacity) + 
                                ",\"maxCapacity\":" + newMaxCapacity + ",\"price\":" + newPrice + 
                                ",\"status\":" + std::to_string(status) + "}";
        log_m.str("");
        log_m << "the admin with the id " << id << " modified a room with number: " << RoomNum 
              << "new max capacity: " << newMaxCapacity << "new price: " << newPrice << " successfully." << std::endl;
        logMessage(log_m.str());
        send(fd, message.c_str(), message.size(), 0);
        data.write_modifiedroom(jsondata);
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
            log_m.str("");
            log_m << "the admin with id " << id << " tried to remove room but results in ERR503." << std::endl;
            logMessage(log_m.str());
            send(fd, message.c_str(), message.size(), 0);
            return;
        }

        //check if the fields are numbers
        if (checkIsANumber(RoomNum, fd) == false)
        {
            message = ERR503;
            message += "/" + std::to_string(id) + "/admin";
            std::cout << "Admin id: " << id << " tried to remove room." << std::endl;
            log_m.str("");
            log_m << "the admin with id " << id << " tried to remove room but results in ERR503." << std::endl;
            logMessage(log_m.str());
            send(fd, message.c_str(), message.size(), 0);
            return;
        }

        //check if room number doesn't exist
        if(check_room_exist(RoomNum) == false)
        {
            message = ERR101;
            message += "/" + std::to_string(id) + "/admin";
            std::cout << "Admin id: " << id << " tried to remove room." << std::endl;
            log_m.str("");
            log_m << "the admin with id " << id << " tried to remove a room with number: " << RoomNum << " but results in ERR101." << std::endl;
            logMessage(log_m.str());
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
                    log_m.str("");
                    log_m << "the admin with id " << id << " tried to remove a room with number: " << RoomNum << " but results in ERR109." << std::endl;
                    logMessage(log_m.str());
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
        std::string jsondata = "{\"number\":\"" + RoomNum + "\"}";
        log_m.str("");
        log_m << "the admin with id " << id << " removed a room with number: " << RoomNum << " successfully." << std::endl;
        logMessage(log_m.str());
        send(fd, message.c_str(), message.size(), 0);
        data.write_deleteroom(jsondata);
        return;
    }
    else
    {
        std::cout << "Admin id: " << id << " tried to edit rooms." << std::endl;
        log_m.str("");
        log_m << "the admin with id " << id << " tried to edit room but results in ERR503." << std::endl;
        logMessage(log_m.str());
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
            log_m.str("");
            log_m << "the user/admin with the id " << id << " logged out." << std::endl;
            logMessage(log_m.str());
            send(fd, ERR201, strlen(ERR201), 0);
            break;
        }
    }
}

std::string Server::cancel(int id, std::istringstream& ss)
{
    std::stringstream ss2;
    std::string room_num, n_person;
    std::getline(ss, room_num, '/');
    std::getline(ss, n_person, '/');

    //check if there is empty field
    if (room_num == "" || n_person == "")
    {
        ss2 << ERR401 << std::endl << "/" << id << "/user";
        log_m.str("");
        log_m << "the user with the id " << id << " tried to cancel a reservation but results in ERR401." << std::endl;
        logMessage(log_m.str());
        return ss2.str();
    }

    //check if the fields are numbers
    if (checkIsANumber(room_num, 0) == false || checkIsANumber(n_person, 0) == false)
    {
        ss2 << ERR401 << std::endl << "/" << id << "/user";
        log_m.str("");
        log_m << "the user with the id " << id << " tried to cancel a reservation but results in ERR401." << std::endl;
        logMessage(log_m.str());
        return ss2.str();
    }
    
    for (int i = 0; i < data.rooms.size(); i++)
    {
        if (data.rooms[i]->getnum() == room_num)
        {
            int cost = data.rooms[i]->getprice();
            int cost_back = cost * stoi(n_person) / 2;
            for (int j = 0; j < data.rooms[i]->getusers().size(); j++)
            {
                if (data.rooms[i]->getusers()[j].id == id)
                {
                    if (data.rooms[i]->getusers()[j].numOfBeds < stoi(n_person))
                    {
                        //over limit...
                        ss2 << ERR102 << std::endl << "/" << id << "/user";
                        std::cout << "User id: " << id << " tried to cancel reservation." << std::endl;
                        log_m.str("");
                        log_m << "the user with the id " << id << " tried to cancel a reservation with the room num: " << room_num 
                              << " for " << n_person << "person but results in ERR102." << std::endl;
                        logMessage(log_m.str());
                        return ss2.str();
                    }
                    else
                    {
                        //cash back and delete reservation
                        for (int k = 0; k < data.users.size(); k++)
                        {
                            if (data.users[k]->getid() == id)
                            {
                                data.users[k]->cash_back(cost_back);
                                std::string jsoncash = "{\"id\":" + std::to_string(id) + ",\"purse\":\"" + data.users[k]->getpurse() + "\"}";
                                data.write_purse(jsoncash);
                            }
                        }
                        
                        //check if the n_person is equal to the number of beds
                        if (data.rooms[i]->getusers()[j].numOfBeds == stoi(n_person))
                        {
                            data.rooms[i]->del_reservation(j);
                            data.rooms[i]->change_capacity(-stoi(n_person));
                            data.rooms[i]->set_status(0);
                            std::cout << "User id: " << id << " canceled reservation." << std::endl;
                            std::string usrjson = "{\"id\":" + std::to_string(id) + ",\"number\":\"" + room_num + 
                                                    "\",\"capacity\":" + std::to_string(data.rooms[i]->getcapacity()) + ",\"status\":" + std::to_string(data.rooms[i]->getstatus()) + "}";
                            data.write_cancel(usrjson);
                            ss2 << ERR110 << std::endl << "/" << id << "/user";
                            log_m.str("");
                            log_m << "the user with the id " << id << " canceled a reservation with the room num: " << room_num 
                                  << " for " << n_person << "person." << std::endl;
                            logMessage(log_m.str());
                            return ss2.str();
                        }
                        else
                        {
                            int num = data.rooms[i]->getusers()[j].numOfBeds;
                            data.rooms[i]->set_numOfBeds(j, num - stoi(n_person));
                            if(!compare_date(data.rooms[i]->getusers()[j].reserveDate) && compare_date(data.rooms[i]->getusers()[j].checkoutDate))
                            {
                                //change the status and capacity if canceling the reservation which is being held.
                                data.rooms[i]->change_capacity(-stoi(n_person));
                                if(data.rooms[i]->getcapacity() < data.rooms[i]->getmax_capacity())
                                    data.rooms[i]->set_status(0);
                            }
                            std::cout << "User id: " << id << " canceled reservation." << std::endl;
                            std::string usrjson = "{\"id\":" + std::to_string(id) + ",\"numOfBeds\":" + std::to_string(data.rooms[i]->get_numOfBeds(j))+ 
                                                    ",\"number\":\"" + room_num + "\",\"status\":" + std::to_string(data.rooms[i]->getstatus()) + ",\"capacity\":" + 
                                                    std::to_string(data.rooms[i]->getcapacity()) + "}";
                            ss2 << ERR110 << std::endl << "/" << id << "/user";
                            data.write_numOfbeds(usrjson);
                            log_m.str("");
                            log_m << "the user with the id " << id << " canceled a reservation with the room num: " << room_num 
                                  << " for " << n_person << "person." << std::endl;
                            logMessage(log_m.str());
                            return ss2.str();
                        }
                    }
                }
            }
            //no reservation found 
            ss2 << ERR102 << std::endl << "/" << id << "/user";
            std::cout << "User id: " << id << " tried to cancel reservation." << std::endl;
            log_m.str("");
            log_m << "the user with the id " << id << " tried to cancel a reservation with the room num: " << room_num 
                  << " for " << n_person << "person but results in ERR102." << std::endl;
            logMessage(log_m.str());
            return ss2.str();

        }
    }
    //the room not found...
    ss2 << ERR101 << std::endl << "/" << id << "/user";
    std::cout << "User id: " << id << " tried to cancel reservation." << std::endl;
    log_m.str("");
    log_m << "the user with the id " << id << " tried to cancel a reservation with the room num: " << room_num 
          << " for " << n_person << "person but results in ERR101." << std::endl;
    logMessage(log_m.str());
    return ss2.str();
}

std::string Server::get_all_reservations(int id)
{
    int count = 1;
    std::stringstream ss;
    for (int i = 0; i < data.rooms.size(); i++)
    {
        for (int j = 0; j < data.rooms[i]->getusers().size(); j++)
        {
            if (data.rooms[i]->getusers()[j].id == id)
            {
                ss << "reservation number " << count << ":" << std::endl;
                ss << "room number: " << data.rooms[i]->getnum() << std::endl;
                ss << "number of reserved beds: " << data.rooms[i]->getusers()[j].numOfBeds << std::endl;
                ss << "from " << data.rooms[i]->getusers()[j].reserveDate << " to " << data.rooms[i]->getusers()[j].checkoutDate << std::endl;
                count++;
            }
        }
    }
    if (count == 1)
    {
        ss << "no reservations yet!" << std::endl;
    }
    ss << "/" << id << "/user" << "/#";
    std::cout << "User id: " << id << " got all reservations." << std::endl;
    log_m.str("");
    log_m << "the user with the id " << id << " tried to cancel a reservation so first the info of all his/her reservations are being showed to him/her. " << std::endl;
    logMessage(log_m.str());
    return ss.str();

}

bool check_interference(std::string from, std::string to, std::string from_, std::string to_)///
{
    // Convert the start and end dates to tm structs
    tm start1 = { 0 };
    strptime(from.c_str(), "%m-%d-%Y", &start1);
    tm end1 = { 0 };
    strptime(to.c_str(), "%m-%d-%Y", &end1);
    tm start2 = { 0 };
    strptime(from_.c_str(), "%m-%d-%Y", &start2);
    tm end2 = { 0 };
    strptime(to.c_str(), "%m-%d-%Y", &end2);

    // Convert tm structs to time_t
    time_t start1_time = mktime(&start1);
    time_t end1_time = mktime(&end1);
    time_t start2_time = mktime(&start2);
    time_t end2_time = mktime(&end2);

    // Check for overlap
    if (end1_time < start2_time || end2_time < start1_time) {
        //no overlap
        return false;
    }
    else {
        //overlap
        return true;
    }
}

std::string Server::book(int id, std::istringstream& ss)
{
    std::stringstream ss2;
    std::string room_num, num_of_beds, check_in_date, check_out_date;
    std::getline(ss, room_num, '/');
    std::getline(ss, num_of_beds, '/');
    std::getline(ss, check_in_date, '/');
    std::getline(ss, check_out_date, '/');

    //check if there is empty field
    if (room_num == "" || num_of_beds == "" || check_in_date == "" || check_out_date == "")
    {
        ss2 << ERR503 << std::endl << "/" << id << "/user";
        log_m.str("");
        log_m << "the user with the id " << id << " tried to book a room but results ERR503. " << std::endl;
        logMessage(log_m.str());
        return ss2.str();
    }

    //check if the fields are numbers
    if (checkIsANumber(room_num, 0) == false || checkIsANumber(num_of_beds, 0) == false || checkDateFormat(check_in_date, false) == false || checkDateFormat(check_out_date, false) == false)
    {
        ss2 << ERR503 << std::endl << "/" << id << "/user";
        log_m.str("");
        log_m << "the user with the id " << id << " tried to book a room but results ERR503. " << std::endl;
        logMessage(log_m.str());
        return ss2.str();
    }

    for (int i = 0; i < data.users.size(); i++)
    {
        if (id == data.users[i]->getid())
        {
            int purse = stoi(data.users[i]->getpurse());
            for (int j = 0; j < data.rooms.size(); j++)
            {
                if (data.rooms[j]->getnum() == room_num)
                {
                    int room_price_per_person = data.rooms[j]->getprice();
                    int cost = stoi(num_of_beds) * room_price_per_person;
                    if (cost > purse)
                    {
                        ss2 << ERR108 << std::endl << "/" << id << "/user";
                        log_m.str("");
                        log_m << "the user with the id " << id << " tried to book a room with number: " << room_num << " for " 
                              << num_of_beds << " person from " << check_in_date << " to " << check_out_date << " but results ERR108." << std::endl;
                        logMessage(log_m.str());
                        return ss2.str();
                    }
                    else
                    {
                        //check if has space ...
                        bool no_space = false;
                        int capacity = data.rooms[j]->getmax_capacity();
                        if (stoi(num_of_beds) > capacity)
                        {
                            no_space = true;
                        }
                        //check if have interference
                        for (int k = 0; k < data.rooms[j]->getusers().size(); k++)
                        {
                            std::string from = data.rooms[j]->getusers()[k].reserveDate;
                            std::string to = data.rooms[j]->getusers()[k].checkoutDate;
                            int same_date = check_interference(from, to, check_in_date, check_out_date);
                            if(same_date)
                            {
                                int n_reserved_bed = data.rooms[j]->getusers()[k].numOfBeds;
                                int beds_needed = n_reserved_bed + stoi(num_of_beds);
                                if(beds_needed > capacity)
                                {
                                    no_space = true;
                                    break;
                                }
                            }
                        }
                        if (no_space)
                        {
                            ss2 << ERR109 << std::endl << "/" << id << "/user";
                            log_m.str("");
                            log_m << "the user with the id " << id << " tried to book a room with number: " << room_num << " for " 
                                  << num_of_beds << " person from " << check_in_date << " to " << check_out_date << " but results ERR109." << std::endl;
                            logMessage(log_m.str());
                            return ss2.str();
                        }
                        else
                        {
                            //change the capacity of the room and add the reservation and reduce the purse of user
                            //capacity change just if the duration of this reservation is now!
                            //what if the date is for before the sys time???
                            data.rooms[j]->add_user(id, num_of_beds, check_in_date, check_out_date);
                            data.users[i]->setpurse(std::to_string(purse - cost));
                            std::string jsonpurse = "{\"purse\":\"" + std::to_string(purse - cost) + "\",\"id\":" + std::to_string(id) + "}";
                            data.write_purse(jsonpurse);
                            int not_started = compare_date(check_in_date);
                            if (!not_started)//if true no change in capacity... else change the capacity..
                            {
                                std::cout << "is started ...\n";
                                data.rooms[j]->change_capacity(stoi(num_of_beds));
                                if(data.rooms[j]->getcapacity() == 0)
                                    data.rooms[j]->set_status(1);
                            }
                            ss2 << ERR110 << std::endl << "/" << id << "/user";
                            std::cout << "User id: " << id << " reserved room number: " << room_num << std::endl;
                            std::string json_userdata = "{\"id\":" + std::to_string(id) + ",\"numOfBeds\":" + 
                                                    num_of_beds + ",\"reserveDate\":\"" + check_in_date + "\",\"checkoutDate\":\"" + 
                                                    check_out_date + "\"}";
                            std::string jsondata = "{\"number\":\"" + room_num + "\",\"status\":" + std::to_string(data.rooms[j]->getstatus()) + 
                                                    ",\"capacity\":" + std::to_string(data.rooms[j]->getcapacity()) + ",\"users\":" + json_userdata + "}";
                            data.write_booking(jsondata);
                            log_m.str("");
                            log_m << "the user with the id " << id << " successfully book a room with number: " << room_num << " for " 
                                  << num_of_beds << " person from " << check_in_date << " to " << check_out_date << "." << std::endl;
                            logMessage(log_m.str());
                            return ss2.str();
                        }
                    }
                }
            }
            //the room not found...
            ss2 << ERR101 << std::endl << "/" << id << "/user";
            log_m.str("");
            log_m << "the user with the id " << id << " tried to book a room with number: " << room_num << " for " 
                  << num_of_beds << " person from " << check_in_date << " to " << check_out_date << " but results ERR101." << std::endl;
            logMessage(log_m.str());
            return ss2.str();
        }
    }
    return "";
}

std::string Server::rooms_info_gathering(int id, std::string command)
{
    std::stringstream ss;
    bool is_admin = check_if_is_admin(id);
    ss << "###########################" << std::endl;
    ss << "rooms info: \n";
    for (int i = 0; i < data.rooms.size(); i++)
    {
        //user want to see rooms that are not full
        if (command == "1")
        {
            if (data.rooms[i]->getstatus() == 0)
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
        }
        else
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
    }
    ss << "/" << id;
    if (is_admin)
    {
        ss << "/admin";
        log_m.str("");
        log_m << "the admin with the id " << id << " wanted to see all rooms info. and the info of all rooms being showed." << std::endl;
        logMessage(log_m.str());
    }
    else
    {
        ss << "/user";
        log_m.str("");
        log_m << "the user with the id " << id << " wanted to see all rooms info. and the info of all rooms being showed." << std::endl;
        logMessage(log_m.str());
    }
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
    ss << "/" << id << "/admin";
    std::string info;
    info = ss.str();
    log_m.str("");
    log_m << "the admin with the id " << id << " wanted to view all users info. and all info being showed." << std::endl;
    logMessage(log_m.str());
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
            log_m.str("");
            log_m << "the user with the id " << id << " wanted to view his/her info." << std::endl;
            logMessage(log_m.str());
            
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
            log_m.str("");
            log_m << "the admin with the id " << id << " wanted to view his/her info" << std::endl;
            logMessage(log_m.str());
            
            ss << "id: " << std::to_string(data.admins[i]->getid()) << std::endl;
            ss << "name: " << data.admins[i]->getname() << std::endl;
            ss << "password: " << data.admins[i]->getpassword() << std::endl;
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
                log_m.str("");
                log_m << "the user with the id " << id << " tried to see all users info but results ERR403." << std::endl;
                logMessage(log_m.str());
            }
            send(fd, info.c_str(), info.size(), 0);
            break;
        }
        case 3: //View rooms information
        {
            std::string rooms_info, command;
            std::getline(ss, command, '/'); //command
            rooms_info = rooms_info_gathering(id, command);
            send(fd, rooms_info.c_str(), rooms_info.size(), 0);
            break;
        }
        case 4: //Booking
        {
            std::string message;
            message = book(id, ss);
            send(fd, message.c_str(), message.size(), 0);
            break;
        }
        case 5: //Canceling
        {
            std::string reservation_info;
            reservation_info = get_all_reservations(id);
            send(fd, reservation_info.c_str(), reservation_info.size(), 0);
            break;
        }
        case 55:
        {
            std::string message;
            message = cancel(id, ss);
            send(fd, message.c_str(), message.size(), 0);
            break;
        }
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
            log_m.str("");
            log_m << "the user with the name " << username << " signed-in." << std::endl;
            logMessage(log_m.str());
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
            log_m.str("");
            log_m << "the admin with the name " << username << " signed-in." << std::endl;
            logMessage(log_m.str());
            send(fd, message.c_str(), message.size(), 0);
            return;
        }
    }
    message = ERR430;
    log_m.str("");
    log_m << "the client with fd " << fd << " tried to signin but results ERR430" << std::endl;
    logMessage(log_m.str());
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
        log_m.str("");
        log_m << "the user " << username << " tried to set password, purse, phoneNumber and address but results ERR503" << std::endl;
        logMessage(log_m.str());
        send(fd, message.c_str(), message.size(), 0);
        return;
    }
    else if(checkIsANumber(phoneNumber, fd) == false)
    {
        std::string message = ERR503;
        log_m.str("");
        log_m << "the user " << username << " tried to set password, purse, phoneNumber and address but results ERR503" << std::endl;
        logMessage(log_m.str());
        send(fd, message.c_str(), message.size(), 0);
        return;
    }
    std::string jsondata = "{\"id\":" + std::to_string(data.users.size()+ data.admins.size()) + ",\"user\":\"" + username + "\",\"password\":\"" + password + "\",\"purse\":\"" + purse + "\",\"phoneNumber\":\"" + phoneNumber + "\",\"address\":\"" + address + "\",\"admin\":\"false\"}";
    data.users.push_back(new User(data.users.size()+ data.admins.size(), username, password, purse, phoneNumber, address));
    
    std::string message = ERR231;
    log_m.str("");
    log_m << "the user " << username << " signed-up successfully by setting some info." << std::endl;
    logMessage(log_m.str());
    send(fd, message.c_str(), message.size(), 0);
    data.write_signedup(jsondata);
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
            log_m.str("");
            log_m << "the client with fd " << fd << " tried to signup but results ERR451" << std::endl;
            logMessage(log_m.str());
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
            log_m.str("");////
            log_m << "the client with fd " << fd << " tried to signup but results ERR451" << std::endl;
            logMessage(log_m.str());
            send(fd, message.c_str(), message.size(), 0);
            return;
        }
    }
    std::stringstream ss;
    ss << ERR311 << "/" << name;
    message = ss.str();
    log_m.str("");
    log_m << "the client with fd " << fd << " signed-up with the name " << name << "." << std::endl;
    logMessage(log_m.str());
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
            log_m.str("");
            log_m << "client with fd " << fd << " tried to signup but results ERR430" << std::endl;
            logMessage(log_m.str());
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
            log_m.str("");
            log_m << "the client with fd " << fd << " tried to signin but results ERR430" << std::endl;
            logMessage(log_m.str());
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

bool Server::checkDateFormat(const std::string& input, bool set_needed) {
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
    if(set_needed)
    {
        set_date(day, month, year);
        log_m.str("");
        log_m << "the system date is being set to " << day << "-" << month << "-" << year << std::endl; 
        logMessage(log_m.str());
    }
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
        logMessage("another server tried to connect to this port but failed to connect!\n");
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
    char buffer[5000] = {0};
    server_fd = setup_server(data.getPort());
    if (server_fd < 0)
    {
        std::cout << "error in building." << std::endl;
        logMessage("error in building server.\n");
        exit(EXIT_FAILURE);
    }
    std::cout << "server is running." << std::endl;
    logMessage("server is running.\n");
    
    std::cout << "enter the date:\n";
    std::string input;
    std::getline(std::cin, input);
    std::stringstream ss(input);
    std::string word;
    ss >> word;
    if(word != "setTime")
    {
        std::cout << ERR503 << std::endl;
        log_m.str("");
        log_m << "tried to set time but results in ERR503. " << std::endl;
        logMessage(log_m.str());
        exit(EXIT_FAILURE);
    }
    std::string date;
    ss >> date;
    if (!checkDateFormat(date, true))
    {
        std::cout << ERR401 << std::endl;
        log_m.str("");
        log_m << "tried to set time but results in ERR401. " << std::endl;
        logMessage(log_m.str());
        exit(EXIT_FAILURE);
    }
    if(ss >> word)
    {
        std::cout << ERR503 << std::endl;
        log_m.str("");
        log_m << "tried to set time but results in ERR503. " << std::endl;
        logMessage(log_m.str());
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
                    log_m.str("");
                    log_m << "new client with fd " << client_fd << " is connected to the server." << std::endl;
                    logMessage(log_m.str());
                    FD_SET(client_fd, &master_set);
                    if (client_fd > max_sd)
                        max_sd = client_fd;
                }
                else //client is sending a message
                {
                    memset(buffer, 0, 5000);
                    int bytes_recieved = recv(i, buffer, 5000, 0);
                    if(bytes_recieved == 0)
                    {
                        std::cout << "client disconnected." << std::endl;
                        log_m.str("");
                        log_m << "client with fd " << i << " is disconnected." << std::endl;
                        logMessage(log_m.str());
                        close(i);
                        FD_CLR(i, &master_set);
                        continue;
                    }
                    log_m.str("");
                    log_m << "new message from client with fd " << i << "." << std::endl;
                    logMessage(log_m.str());
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
