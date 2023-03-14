#include "../library/client.hpp"

Client::Client() {}

std::stringstream log_m;
std::string username_global;

void logMessage(std::string message ,std::string folder_name) 
{
    std::ofstream logfile;
    mkdir(folder_name.c_str(), 0777);
    std::string file_name = folder_name + "/" + folder_name + ".txt";
    logfile.open(file_name, std::ios_base::app); // open the file in append mode
    if (logfile.is_open()) {
        auto time_now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string timestamp = std::ctime(&time_now); // get the current time as a string
        logfile << timestamp.substr(0, timestamp.length()-1) << " - " << message << std::endl;
        logfile.close();
    } else {
        std::cerr << "Failed to open log file" << std::endl;
    }
}

void add_log(int size, std::string token_0, std::string token_2, std::string token_3)
{
    if(size == 4 && token_3 == "1" )
        {
            log_m.str("");
            log_m << username_global << " successfully saw his/her info. " << std::endl;
            logMessage(log_m.str(), username_global);
        }

        if(size == 4 && token_3 == "2")
        {
            if(token_2 == "user")
            {
                log_m.str("");
                log_m << username_global << " tried to see all users info but access denied " << std::endl;
                logMessage(log_m.str(), username_global);
            }
            else
            {
                log_m.str("");
                log_m << username_global << " successfully saw all users info. " << std::endl;
                logMessage(log_m.str(), username_global);
            }
        }

        if(size == 4 && token_3[0] == '3')
        {
            if(token_3 == "31")
            {
                log_m.str("");
                log_m << username_global << " saw all the rooms info which still has empty beds. " << std::endl;
                logMessage(log_m.str(), username_global);
            }
            else
            {
                log_m.str("");
                log_m << username_global << " saw all the rooms info." << std::endl;
                logMessage(log_m.str(), username_global);
            }
        }

        if(size == 4 && token_3 == "4")
        {
            std::cout << "i am in 4\n";
            if(token_0 == ERR503)
            {
                log_m.str("");
                log_m << username_global << " tried to book a room but results ERR503." << std::endl;
                logMessage(log_m.str(), username_global);
            }
            else if (token_0 == ERR108)
            {
                log_m.str("");
                log_m << username_global << " tried to book a room but results ERR108." << std::endl;
                logMessage(log_m.str(), username_global);
            }
            else if (token_0 == ERR109)
            {
                log_m.str("");
                log_m << username_global << " tried to book a room but results ERR109." << std::endl;
                logMessage(log_m.str(), username_global);
            }
            else if (token_0 == ERR110)
            {
                log_m.str("");
                log_m << username_global << " successfully booked a room!" << std::endl;
                logMessage(log_m.str(), username_global);
            }
            else if (token_0 == ERR101)
            {
                log_m.str("");
                log_m << username_global << " tried to book a room but results ERR101." << std::endl;
                logMessage(log_m.str(), username_global);
            }  
        }

        if(size == 4 && token_3 == "5")
        {
            if(token_0 == ERR401)
            {
                log_m.str("");
                log_m << username_global << " tried to cancel a room but results ERR401." << std::endl;
                logMessage(log_m.str(), username_global);
            }
            else if (token_0 == ERR102)
            {
                log_m.str("");
                log_m << username_global << " tried to cancel a room but results ERR102." << std::endl;
                logMessage(log_m.str(), username_global);
            }
            else if (token_0 == ERR110)
            {
                log_m.str("");
                log_m << username_global << " successfully canceled a room!" << std::endl;
                logMessage(log_m.str(), username_global);
            }
            else if (token_0 == ERR101)
            {
                log_m.str("");
                log_m << username_global << " tried to cancel a room but results ERR101." << std::endl;
                logMessage(log_m.str(), username_global);
            }
        }

        if (size == 4 && token_3[0] == '9')
        {
            if (token_3 == "9a")
            {
                if (token_0 == ERR503)
                {
                    log_m.str("");
                    log_m << username_global << " tried to add a room but results ERR503." << std::endl;
                    logMessage(log_m.str(), username_global);
                }
                else if (token_0 == ERR111)
                {
                    log_m.str("");
                    log_m << username_global << " tried to add a room but results ERR111." << std::endl;
                    logMessage(log_m.str(), username_global);
                }
                else if (token_0 == ERR104)
                {
                    log_m.str("");
                    log_m << username_global << " successflly added the room." << std::endl;
                    logMessage(log_m.str(), username_global);
                }   
            }
            else if (token_3 == "9m")
            {
                if (token_0 == ERR503)
                {
                    log_m.str("");
                    log_m << username_global << " tried to modify a room but results ERR503." << std::endl;
                    logMessage(log_m.str(), username_global);
                }
                else if (token_0 == ERR101)
                {
                    log_m.str("");
                    log_m << username_global << " tried to modify a room but results ERR101." << std::endl;
                    logMessage(log_m.str(), username_global);
                }
                else if (token_0 == ERR109)
                {
                    log_m.str("");
                    log_m << username_global << " tried to modify a room but results ERR109." << std::endl;
                    logMessage(log_m.str(), username_global);
                }
                else if (token_0 == ERR105)
                {
                    log_m.str("");
                    log_m << username_global << " successfully modified the room." << std::endl;
                    logMessage(log_m.str(), username_global);
                }                
            }
            else if (token_3 == "9r")
            {
                if (token_0 == ERR503)
                {
                    log_m.str("");
                    log_m << username_global << " tried to remove a room but results ERR503." << std::endl;
                    logMessage(log_m.str(), username_global);
                }
                else if (token_0 == ERR101)
                {
                    log_m.str("");
                    log_m << username_global << " tried to remove a room but results ERR101." << std::endl;
                    logMessage(log_m.str(), username_global);
                }
                else if (token_0 == ERR109)
                {
                    log_m.str("");
                    log_m << username_global << " tried to remove a room but results ERR109." << std::endl;
                    logMessage(log_m.str(), username_global);
                }
                else if (token_0 == ERR106)
                {
                    log_m.str("");
                    log_m << username_global << " successfully removed the room." << std::endl;
                    logMessage(log_m.str(), username_global);
                }   
            }
            else if (token_3 == "9")
            {
                log_m.str("");
                log_m << username_global << " tried to edit a room but results ERR503." << std::endl;
                logMessage(log_m.str(), username_global);
            }  
        }

        if (size == 4 && token_3 == "8")
        {
            if (token_2 == "admin")
            {
                if (token_0 == ERR503)
                {
                    log_m.str("");
                    log_m << username_global << " tried to empty a room but results ERR503." << std::endl;
                    logMessage(log_m.str(), username_global);
                }
                else if (token_0 == ERR413)
                {
                    log_m.str("");
                    log_m << username_global << " successfully make the room empty!" << std::endl;
                    logMessage(log_m.str(), username_global);
                }
                else if (token_0 == ERR412)
                {
                    log_m.str("");
                    log_m << username_global << " tried to empty a room but results ERR412." << std::endl;
                    logMessage(log_m.str(), username_global);
                }
                else if (token_0 == ERR401)
                {
                    log_m.str("");
                    log_m << username_global << " tried to empty a room but results ERR401." << std::endl;
                    logMessage(log_m.str(), username_global);
                }
                else if (token_0 == ERR101)
                {
                    log_m.str("");
                    log_m << username_global << " tried to empty a room but results ERR101." << std::endl;
                    logMessage(log_m.str(), username_global);
                }                
                
            }
            else if (token_2 == "user")
            {
                if (token_0 == ERR503)
                {
                    log_m.str("");
                    log_m << username_global << " tried to leave a room but results ERR503." << std::endl;
                    logMessage(log_m.str(), username_global);
                }
                else if (token_0 == ERR413)
                {
                    log_m.str("");
                    log_m << username_global << " successfully left the room!" << std::endl;
                    logMessage(log_m.str(), username_global);
                }
                else if (token_0 == ERR102)
                {
                    log_m.str("");
                    log_m << username_global << " tried to leave a room but results ERR102." << std::endl;
                    logMessage(log_m.str(), username_global);
                }
            } 
        }

        if (size == 4 && token_3 == "7")
        {
            if (token_0 == ERR503)
            {
                log_m.str("");
                log_m << username_global << " tried to edit info but results ERR503." << std::endl;
                logMessage(log_m.str(), username_global);
            }
            else if (token_0 == ERR312)
            {
                log_m.str("");
                log_m << username_global << " successfully edited the info!" << std::endl;
                logMessage(log_m.str(), username_global);
            } 
        }
        
        if (size == 4 && token_3 == "6")
        {
            if (token_0 == ERR503)
            {
                log_m.str("");
                log_m << username_global << " tried to pass day but results ERR503." << std::endl;
                logMessage(log_m.str(), username_global);
            }
            else if (token_0 == ERR401)
            {
                log_m.str("");
                log_m << username_global << " tried to pass day but results ERR401." << std::endl;
                logMessage(log_m.str(), username_global);
            }
            else if (token_0 == ERR312)
            {
                log_m.str("");
                log_m << username_global << " successfully passed days!" << std::endl;
                logMessage(log_m.str(), username_global);
            }
        }
}

bool checkIsANumber(std::string input)
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

std::string action_sentences(int choice, int id, std::string user_admin)
{
    switch (choice)
    {
        case 1:
            return "menu/1/" + std::to_string(id);
        case 2:
            return "menu/2/" + std::to_string(id);
        case 3:{
            std::cout << "If you want to see rooms that still have free beds, enter 1 else enter 0" << std::endl;
            int order;
            std::cin >> order;
            if (order == 1)
            {
                log_m.str("");
                log_m << username_global << " by choosing option " << order <<" wanted to see the rooms that still have free beds " << std::endl;
                logMessage(log_m.str(), username_global);

                return "menu/3/" + std::to_string(id) + "/1";
            }
            else if (order == 0)
            {
                log_m.str("");
                log_m << username_global << " by choosing option " << order << " wanted to see all the rooms info. " << std::endl;
                logMessage(log_m.str(), username_global);

                return "menu/3/" + std::to_string(id) + "/0";
            } 
            else
            {
                log_m.str("");
                log_m << username_global << " choosed option " << order << " which is not valid. " << std::endl;
                logMessage(log_m.str(), username_global);

                return "error";
            }
        }
        case 4:{
            if(user_admin != "user")
            {
                log_m.str("");
                log_m << username_global << " tried to book a room but access denied. " << std::endl;
                logMessage(log_m.str(), username_global);

                return "error403";
            }
            std::cout << "book <RoomNum> <NumOfBeds> <CheckInDate> <CheckOutDate>" << std::endl;
            std::string command, RoomNum, NumOfBeds, CheckInDate, CheckOutDate, book;
            std::getline(std::cin >> std::ws, command);
            std::stringstream ss(command);
            std::getline (ss, book, ' ');
            if (book != "book")
            {
                log_m.str("");
                log_m << username_global << " tried to book a room but results typo." << std::endl;
                logMessage(log_m.str(), username_global);
                
                return "error";
            }
            std::getline(ss, RoomNum, ' ');
            std::getline (ss, NumOfBeds, ' ');
            std::getline (ss, CheckInDate, ' ');
            std::getline (ss, CheckOutDate, ' ');
            log_m.str("");
            log_m << username_global << " tried to book a room number " << RoomNum << ", with " << NumOfBeds << " beds from "
                  << CheckInDate << " to " << CheckOutDate << "." << std::endl;
            logMessage(log_m.str(), username_global);
            
            return "menu/4/" + std::to_string(id) + "/" + RoomNum + "/" + NumOfBeds + "/" + CheckInDate + "/" + CheckOutDate;
        }
        case 5:{
            if (user_admin != "user")
            {
                log_m.str("");
                log_m << username_global << " tried to cancel a room but access denied. " << std::endl;
                logMessage(log_m.str(), username_global);

                return "error403";
            }
            std::cout << "cancel <RoomNum> <Num>" << std::endl;
            std::string command, RoomNum, Num, cancel;
            std::getline(std::cin >> std::ws, command);
            std::stringstream ss(command);
            std::getline (ss, cancel, ' ');
            if (cancel != "cancel")
            {
                log_m.str("");
                log_m << username_global << " tried to cancel a room but results typo." << std::endl;
                logMessage(log_m.str(), username_global);
                
                return "error";
            }
            std::getline(ss, RoomNum, ' ');
            std::getline (ss, Num, ' ');
            
            log_m.str("");
            log_m << username_global << " tried to cancel a room number " << RoomNum << " for " << Num << " beds." << std::endl;
            logMessage(log_m.str(), username_global);
            
            return "menu/55/" + std::to_string(id) + "/" + RoomNum + "/" + Num;
        }
        case 6:{
            if(user_admin != "admin")
            {
                log_m.str("");
                log_m << username_global << " tried to pass day but access denied. " << std::endl;
                logMessage(log_m.str(), username_global);
                
                return "error403";
            }
            std::cout << "passDay <value>" << std::endl;
            std::string command, passDay, value;
            std::getline(std::cin >> std::ws, command);
            std::stringstream ss(command);
            std::getline (ss, passDay, ' ');
            if (passDay != "passDay")
            {
                log_m.str("");
                log_m << username_global << " tried to pass day but results typo." << std::endl;
                logMessage(log_m.str(), username_global);
                return "error";
            }
            std::getline(ss, value, ' ');
            
            log_m.str("");
            log_m << username_global << " tried to pass day for " << value << " days." << std::endl;
            logMessage(log_m.str(), username_global);
            
            return "menu/6/" + std::to_string(id) + "/" + value + "/" + user_admin;
        }   
        case 7:{
            std::string password, phone, address;
            if (user_admin == "admin")
            {
                std::cout << "new password" << std::endl;
                std::cin >> password;
                
                log_m.str("");
                log_m << username_global << " tried to edit his/her password to " << password << "." << std::endl;
                logMessage(log_m.str(), username_global);
                
                return "menu/7/" + std::to_string(id) + "/" + password;
            }
            else if(user_admin == "user")
            {
                std::cout << "new password" << std::endl;
                std::cin >> password;
                std::cout << "phone" << std::endl;
                std::cin >> phone;
                std::cout << "address" << std::endl;
                std::cin >> address;
                
                log_m.str("");
                log_m << username_global << " tried to edit info with new password " << password << ", phone "
                      << phone << ", address " << address << "." << std::endl;
                logMessage(log_m.str(), username_global);
                
                return "menu/7/" + std::to_string(id) + "/" + password + "/" + phone + "/" + address;
            }
        }
        case 8:{
            if(user_admin == "user")
            {
                std::cout << "room <value>" << std::endl;
                std::string command, room, value;
                std::getline(std::cin >> std::ws, command);
                std::stringstream ss(command);
                std::getline (ss, room, ' ');
                if (room != "room")
                {
                    log_m.str("");
                    log_m << username_global << " tried to leave a room but results typo." << std::endl;
                    logMessage(log_m.str(), username_global);

                    return "error";
                }
                std::getline(ss, value, ' ');
                
                log_m.str("");
                log_m << username_global << " tried to leave a room number " << value << "." << std::endl;
                logMessage(log_m.str(), username_global);
                
                return "menu/8/" + std::to_string(id) + "/" + room + "/" + value;
            }
            else if(user_admin == "admin")
            {
                std::cout << "room <value>" << std::endl;
                std::string command, roomOrCapacity, value, command2, roomOrCapacity2 , value2;
                std::getline(std::cin >> std::ws, command);
                std::stringstream ss(command);
                std::getline (ss, roomOrCapacity, ' ');
                if (roomOrCapacity != "room")
                {
                    log_m.str("");
                    log_m << username_global << " tried to empty a room but results typo." << std::endl;
                    logMessage(log_m.str(), username_global);

                    return "error";
                }
                std::getline(ss, value, ' ');
                std::cout << "capacity <new capacity>" << std::endl;
                std::getline(std::cin >> std::ws, command2);
                std::stringstream ss2(command2);
                std::getline (ss2, roomOrCapacity2, ' ');
                if (roomOrCapacity2 != "capacity")
                {
                    log_m.str("");
                    log_m << username_global << " tried to empty a room but results typo." << std::endl;
                    logMessage(log_m.str(), username_global);

                    return "error";
                }
                std::getline(ss2, value2, ' ');
                
                log_m.str("");
                log_m << username_global << " tried to empty a room number " << value << " by " << value2 << " beds." << std::endl;
                logMessage(log_m.str(), username_global);
                
                return "menu/8/" + std::to_string(id) + "/" + value + "/" + value2;
            }
        }
        case 9:{
            if(user_admin == "user")
            {
                log_m.str("");
                log_m << username_global << " tried to edit rooms but access denied. " << std::endl;
                logMessage(log_m.str(), username_global);

                return "error403";
            }
            std::cout << "add <RoomNum> <Max Capacity> <Price>" << std::endl;
            std::cout << "modify <RoomNum> <New Max Capacity> <New Price>" << std::endl;
            std::cout << "remove <RoomNum>" << std::endl;
            std::string command, order, RoomNum, MaxCapacity, Price;
            std::getline(std::cin >> std::ws, command);
            std::stringstream ss(command);
            std::getline (ss, order, ' ');
            if(order == "add")
            {
                std::getline(ss, RoomNum, ' ');
                std::getline (ss, MaxCapacity, ' ');
                std::getline (ss, Price, ' ');
                
                log_m.str("");
                log_m << username_global << " tried to add a room number " << RoomNum << " with max capacity "
                      << MaxCapacity << " and price " << Price << "." << std::endl;
                logMessage(log_m.str(), username_global);
                
                return "menu/9/" + std::to_string(id) + "/add/" + RoomNum + "/" + MaxCapacity + "/" + Price;
            }
            else if(order == "modify")
            {
                std::getline(ss, RoomNum, ' ');
                std::getline (ss, MaxCapacity, ' ');
                std::getline (ss, Price, ' ');
                
                log_m.str("");
                log_m << username_global << " tried to modify a room number " << RoomNum << " with max capacity "
                      << MaxCapacity << " and price " << Price << "." << std::endl;
                logMessage(log_m.str(), username_global);
                
                return "menu/9/" + std::to_string(id) + "/modify/" + RoomNum + "/" + MaxCapacity + "/" + Price;
            }
            else if(order == "remove")
            {
                std::getline(ss, RoomNum, ' ');
                
                log_m.str("");
                log_m << username_global << " tried to remove a room number " << RoomNum << "." << std::endl;
                logMessage(log_m.str(), username_global);
                
                return "menu/9/" + std::to_string(id) + "/remove/" + RoomNum;
            }
            else
            {
                log_m.str("");
                log_m << username_global << " tried to edit a room but results typo." << std::endl;
                logMessage(log_m.str(), username_global);

                return "error";
            }
        }
        case 0:
        {
            log_m.str("");
            log_m << username_global << " tried to logout." << std::endl;
            logMessage(log_m.str(), username_global);

            return "menu/0/" + std::to_string(id);
        }
    }
    return "";
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

std::string user_list(int id, std::string user_admin)
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
        
        log_m.str("");
        log_m << username_global << " order choice number " << choice_num << std::endl;
        logMessage(log_m.str(), username_global);
        
        if (!isNumberBetween0And9(choice_num))
        {
            std::cout << ERR503 << std::endl;
            
            log_m.str("");
            log_m << username_global << " order choice number " << choice_num << " but the number was out of range." << std::endl;
            logMessage(log_m.str(), username_global);
            
            continue;               
        }
        else
        {
            if (stoi(choice_num) == 5)
            {
                if (user_admin != "user")
                {
                    log_m.str("");
                    log_m << username_global << " order choice number " << choice_num << "but access denied." << std::endl;
                    logMessage(log_m.str(), username_global);
                    
                    return "error403";
                }
                return "menu/5/" + std::to_string(id);
            }
            return action_sentences(stoi(choice_num),id, user_admin);
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
    username_global = new_username;
    
    log_m.str("");
    log_m << username_global << " signed-up and tried to set password as " << password << ", purse as " << purse 
          << ", phone number as " << phoneNumber << " and address as " << address << "." << std::endl;
    
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
    else if(order == "exit")
    {
        std::cout << "Goodbye!" << std::endl;
        exit(0);
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
    std::cout << "exit" << std::endl;

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
    char buffer[5000] = {0};
    int id;
    std::string user_admin;
    bool flag_cancel = 0;
    bool flag_inner_error = 0;
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
        if(flag_inner_error == 0)
            add_log(tokens.size(), tokens[0], tokens[2], tokens[3]);
        
        flag_inner_error = 0;
        
        if(!tokens.empty() && tokens[0] == ERR231)
        {
            logMessage(log_m.str(), username_global);
        }
        
        //logged in list
        if(!tokens.empty() && tokens[0] == ERR230) 
        {
            id = stoi(tokens[1]);
            user_admin = tokens[2];
            username_global = tokens[3];
            
            log_m.str("");
            log_m << username_global << " successfully logged-in. " << std::endl;
            logMessage(log_m.str(), username_global);
            
            command = user_list(id, user_admin);
            if (command == "error" && str != ERR503)
            {
                std::cout << ERR503 << std::endl;
                flag_inner_error = 1;
                continue;
            }
            if(command == "error403")
            {
                std::cout << ERR403 << std::endl;
                flag_inner_error = 1;
                continue;
            }
        }
        
        else if(tokens.size() > 1 && tokens[0] != ERR311)
        {
            if(checkIsANumber(tokens[1]))
            {
                id = stoi(tokens[1]);
                user_admin = tokens[2];
                if (tokens.size() == 4 && tokens[3] == "#")
                {
                    flag_cancel = 1;
                    command = action_sentences(5, id, user_admin);
                }
                else
                    command = user_list(id, user_admin);
                if (command == "error" && str != ERR503)
                {
                    std::cout << ERR503 << std::endl;
                    flag_inner_error = 1;
                    continue;
                }
                if(command == "error403")
                {
                    std::cout << ERR403 << std::endl;
                    flag_inner_error = 1;
                    continue;
                }
                flag_cancel = 0;
            }
        }

        //sign up
        else if (tokens.empty() || tokens[0] != ERR311)
        {
            command = show_list();
            command = define_command(command);
            if (command == "error" && str != ERR503)
            {
                std::cout << ERR503 << std::endl;
                flag_inner_error = 1;
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
        read(fd, buffer, 5000);
        ss.clear();
        ss.str(buffer);
        tokens.clear();
        while (std::getline(ss, token, '/')){
            tokens.push_back(token);
        }
        std::cout << tokens[0] << std::endl;
    }
}

int main(int argc, char const *argv[])
{
    readJson data;
    Client client;
    client.build();
    return 0;
}
