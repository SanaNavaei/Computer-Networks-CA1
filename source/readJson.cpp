#include "../library/readJson.hpp"
int readJson::getPort()
{
    return port;
}
std::string readJson::getHostName()
{
    return hostName;
}

//write signedup user in Userinfo.json
void readJson::write_signedup(std::string data)
{
    std::ifstream user_file("./UserInfo.json");

    json user_data = json::parse(user_file);
    user_file.close();

    user_data["users"].push_back(json::parse(data));
    std::ofstream file("./UserInfo.json");
    file << std::setw(4) << user_data;
    file.close();
}

//write edit info of user in Userinfo.json
void readJson::write_editinfo(std::string data)
{
    std::ifstream user_file("./UserInfo.json");

    json user_data = json::parse(user_file);
    user_file.close();

    for (auto& user : user_data["users"])
    {
        if (user["id"] == json::parse(data)["id"])
        {
            if (user["admin"] == "true")
            {
                user["password"] = json::parse(data)["password"];
                continue;
            }
            user["password"] = json::parse(data)["password"];
            user["phoneNumber"] = json::parse(data)["phoneNumber"];
            user["address"] = json::parse(data)["address"];
        }
    }
    std::ofstream file("./UserInfo.json");
    file << std::setw(4) << user_data;
    file.close();
}

//remove user from room in RoomInfo.json
void readJson::write_leaveroom(std::string data)
{
    std::ifstream room_file("./RoomsInfo.json");

    json room_data = json::parse(room_file);
    room_file.close();

    for (auto& room : room_data["rooms"])
    {
        json newarray = json::array();
        if (room["number"] == json::parse(data)["number"])
        {
            for (auto& user : room["users"])
            {
                if (user["id"] == json::parse(data)["id"])
                {
                    //set new capacity of room
                    room["capacity"] = json::parse(data)["capacity"];
                    room["status"] = json::parse(data)["status"];
                    user.clear();
                }
                else
                    newarray.emplace_back(user);
            }
            room["users"] = newarray;
        }
    }
    
    std::ofstream file("./RoomsInfo.json");
    file << std::setw(4) << room_data;
    file.close();
}

//add room in RoomInfo.json
void readJson::write_addroom(std::string data)
{
    std::ifstream room_file("./RoomsInfo.json");

    json room_data = json::parse(room_file);
    room_file.close();

    room_data["rooms"].push_back(json::parse(data));
    std::ofstream file("./RoomsInfo.json");
    file << std::setw(4) << room_data;
    file.close();
}

//modify room in RoomInfo.json
void readJson::write_modifiedroom(std::string data)
{
    std::ifstream room_file("./RoomsInfo.json");

    json room_data = json::parse(room_file);
    room_file.close();

    for(auto& room : room_data["rooms"])
    {
        if(room["number"] == json::parse(data)["number"])
        {
            room["capacity"] = json::parse(data)["capacity"];
            room["price"] = json::parse(data)["price"];
            room["maxCapacity"] = json::parse(data)["maxCapacity"];
            room["status"] = json::parse(data)["status"];
        }
    }
    std::ofstream file("./RoomsInfo.json");
    file << std::setw(4) << room_data;
    file.close();
}

//delete room in RoomInfo.json
void readJson::write_deleteroom(std::string data)
{
    std::ifstream room_file("./RoomsInfo.json");

    json room_data = json::parse(room_file);
    room_file.close();

    json newarray = json::array();
    for(auto& room : room_data["rooms"])
    {
        if(room["number"] != json::parse(data)["number"])
        {
            newarray.emplace_back(room);
        }
    }
    room_data["rooms"] = newarray;
    std::ofstream file("./RoomsInfo.json");
    file << std::setw(4) << room_data;
    file.close();
}

//add user to room in RoomInfo.json
void readJson::write_booking(std::string data)
{
    std::ifstream room_file("./RoomsInfo.json");

    json room_data = json::parse(room_file);
    room_file.close();

    for (auto& room : room_data["rooms"])
    {
        if (room["number"] == json::parse(data)["number"])
        {
            room["capacity"] = json::parse(data)["capacity"];
            room["status"] = json::parse(data)["status"];
            room["users"].push_back(json::parse(data)["users"]);
        }
    }
    std::ofstream file("./RoomsInfo.json");
    file << std::setw(4) << room_data;
    file.close();
}

//update purse of user in Userinfo.json
void readJson::write_purse(std::string data)
{
    std::ifstream user_file("./UserInfo.json");

    json user_data = json::parse(user_file);
    user_file.close();

    for (auto& user : user_data["users"])
    {
        if (user["id"] == json::parse(data)["id"])
        {
            user["purse"] = json::parse(data)["purse"];
        }
    }
    std::ofstream file("./UserInfo.json");
    file << std::setw(4) << user_data;
    file.close();
}

//cancel booking in RoomInfo.json
void readJson::write_cancel(std::string data)
{
    std::ifstream room_file("./RoomsInfo.json");

    json room_data = json::parse(room_file);
    room_file.close();

    for (auto& room : room_data["rooms"])
    {
        json newarray = json::array();
        if (room["number"] == json::parse(data)["number"])
        {
            for (auto& user : room["users"])
            {
                if (user["id"] == json::parse(data)["id"])
                    user.clear();
                else
                    newarray.emplace_back(user);
            }
            room["users"] = newarray;
        }
    }
    std::ofstream file("./RoomsInfo.json");
    file << std::setw(4) << room_data;
    file.close();
}

//update numOfbeds in RoomInfo.json
void readJson::write_numOfbeds(std::string data)
{
    std::ifstream room_file("./RoomsInfo.json");

    json room_data = json::parse(room_file);
    room_file.close();

    for (auto& room : room_data["rooms"])
    {
        if (room["number"] == json::parse(data)["number"])
        {
            for (auto& user : room["users"])
            {
                if (user["id"] == json::parse(data)["id"])
                {
                    user["numOfBeds"] = json::parse(data)["numOfBeds"];
                }
            }
        }
    }
    std::ofstream file("./RoomsInfo.json");
    file << std::setw(4) << room_data;
    file.close();
}


//change capacity of room in RoomInfo.json
void readJson::write_capacity(std::string data)
{
    std::ifstream room_file("./RoomsInfo.json");

    json room_data = json::parse(room_file);
    room_file.close();

    for (auto& room : room_data["rooms"])
    {
        if (room["number"] == json::parse(data)["number"])
        {
            room["capacity"] = json::parse(data)["capacity"];
            room["status"] = json::parse(data)["status"];
            room["users"].clear();
        }
    }
    std::ofstream file("./RoomsInfo.json");
    file << std::setw(4) << room_data;
    file.close();
}

readJson::readJson()
{
    //Read config.json
    std::ifstream config_file("./config.json");
    json config_data = json::parse(config_file);
    hostName = config_data["hostName"];
    port = config_data["commandChannelPort"];

    //Read userInfo.json
    std::ifstream user_file("./UserInfo.json");
    json user_data = json::parse(user_file);

    for (auto& user : user_data["users"])
    {
        int id = user["id"];
        std::string username = user["user"];
        std::string password = user["password"];
        std::string admin = user["admin"];
        if (admin == "false")
        {
            std::string purse = user["purse"];
            std::string phoneNumber = user["phoneNumber"];
            std::string address = user["address"];
            users.push_back(new User(id, username, password, purse, phoneNumber, address));
        }
        else
        {
            admins.push_back(new Admin(id, username, password));
        }
    }

    //Read RoomInfo.json
    std::ifstream room_file("./RoomsInfo.json");
    json room_data = json::parse(room_file);
    
    for (auto& room : room_data["rooms"])
    {
        std::string number = room["number"];
		int status = room["status"];
		int price = room["price"];
		int maxCapacity = room["maxCapacity"];
		int capacity = room["capacity"];

		//vector of struct to store data of users in room
        	std::vector<userInRoom> userInRooms;
		for (auto& user : room["users"])
		{
			userInRoom userInRoom;
            		userInRoom.id = user["id"];
            		userInRoom.numOfBeds = user["numOfBeds"];
            		userInRoom.reserveDate = user["reserveDate"];
            		userInRoom.checkoutDate = user["checkoutDate"];
            		userInRooms.push_back(userInRoom);
    		}
		rooms.push_back(new Rooms(number, status, price, maxCapacity, capacity, userInRooms));
	}
}
