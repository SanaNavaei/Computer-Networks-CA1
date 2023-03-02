#include "../library/readJson.hpp"
int readJson::getPort()
{
    return port;
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