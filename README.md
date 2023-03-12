# CN_CHomeworks_1

**سنا ساری نوایی : 810199435**   
**مریم جعفرآبادی آشتیانی : 810199549**
<br/>
<br/>

## توضیحات اولیه راجع به منطق برنامه 
در این پروژه، به پیاده سازی یک سرور هتل می‌پردازیم. انواع درخواست‌ها از قبیل، رزرو، کنسل کردن رزرو، ویرایش اطلاعات و اطلاعات دیگر صورت میگیرد. برای هر موجودیت در برنامه کلاس های جداگانه در نظر گرفته شده است. که به شرح هرکدام میپردازیم.
<br/>

- readJson
- Server
- Client
- User
- Admin
- Rooms

### اول از توضیح کلاس `readJson` شروع میکنیم:  
این کلاس، کلاسی است که برنامه های ما با آن شروع میشود. داده های ما به صورت 3 فایل json است که RoomsInfo.json، config.json و UserInfo.json نام دارند. این کلاس اطلاعات را از این فایل ها دریافت میکند. `port` و `hostName` از فیلد های پرایوت کلاس هستند و توابع پابلیک این بخش برای نوشتن در فایل های جیسون پس از تغییر اطلاعات استفاده میشود. 


```cpp
class readJson
{
public:
    readJson();

    void write_signedup(std::string data);
    void write_editinfo(std::string data);
    void write_leaveroom(std::string data);
    void write_addroom(std::string data);
    void write_modifiedroom(std::string data);
    void write_deleteroom(std::string data);
    void write_booking(std::string data);
    void write_purse(std::string data);
    void write_cancel(std::string data);
    void write_numOfbeds(std::string data);
    void write_capacity(std::string data);

    int getPort();

    std::string getHostName();
    std::vector<User*> users;
    std::vector<Admin*> admins;
    std::vector<Rooms*> rooms;
private:
    std::string hostName;
    int port;
};
```

<br/>
<br/>

### کلاس `Server`:
این کلاس وظیفه ی هندل کردن سوکت ها را دارد. از طرف دیگر، توابعی که در این کلاس وجود دارند، دستور های مورد نظر ما را اجرا میکنند. 
    
```cpp
class Server
{
public:
    Server(readJson data);
    
    int setup_server(int port);
    
    void build();
    void signup(std::string username, std::string password, std::string purse, std::string phoneNumber, std::string address, int fd);
    void checkCommand(char buff[], int fd);
    void set_date(std::string day_, std::string month_, std::string year_);
    void checkusername(std::string name, int fd);
    void signin(std::string username, std::string password, int fd);
    void action_to_be_done(int choice, int id, int fd, std::istringstream& ss);
    void logout(int id, int fd);
    void edit_information(int id, int fd, std::istringstream &ss);
    void edit_rooms(int id, int fd, std::istringstream &ss);
    void leave_room(int id, int fd, std::istringstream &ss);
    void pass_day(int id, int fd, std::istringstream &ss);
    void handle_pass_day();
    void user_leave_room(int id, int fd, std::istringstream& ss);
    void admin_leave_room(int id, int fd, std::istringstream &ss);

    std::string get_info(int id);
    std::string user_info_gathering(int id);
    std::string rooms_info_gathering(int id, std::string command);
    std::string book(int id, std::istringstream& ss);
    std::string cancel(int id, std::istringstream& ss);
    std::string get_all_reservations(int id);

    bool checkDateFormat(const std::string& input, bool set_neede);
    bool checkIsANumber(std::string input, int fd);
    bool check_if_is_admin(int id);
    bool check_room_exist(std::string room_number);
    bool compare_date(std::string date);
private:
    
    readJson data;
    Date sys_date;
    std::vector<int> loggedInIds;
};
```
#### به بررسی توابع مهم در کلاس سرور میپردازیم:
<br/>
<br/>

تابع `()build` از مهمترین توابع در کلاس سرور میباشد. در اوایل این تابع، تابع `()setup_server` قرار دارد. این تابع در شکل زیر آمده است. با استفاده از سوکت، یک ارتباط TCP میسازیم. سپس bind میکنیم که به پورتی که دریافت کرده ایم، وصل شویم. در نهایت listen میکنیم تا درخواست ها از سمت کلاینت دریافت شود.  

```cpp
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
```
سپس به یک حلقه ی بی نهایت میرسیم که دو اتفاق ممکن است اتفاق بیفتد:  
یک: کلاینت جدیدی به سرور متصل شود. در صورتی که کلاینت جدید وارد شود، تابع `()acceptClient` فراخوانی میشود. این تابع بعدا توضیح داده میشود.  
دو: کلاینتی که قبلا متصل بوده، پیامی ارسال کند. در این حالت اگر تعداد بیتی که از کاربر دریافت شده صفر باشد، به این معناست که کاربر خارج شده و اطلاعاتش حذف میشود. در غیر اینصورت، وارد تابع `()checkCommand` میشود که خواسته های کاربر را بررسی میکند.

```cpp
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
                    memset(buffer, 0, 5000);
                    int bytes_recieved = recv(i, buffer, 5000, 0);
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
```

تابع مهم بعدی، `()action_to_be_done` است. در این تابع، با توجه به کامند های مورد نظر، تابع مربوط به هر دستور که کاربر درخواست کرده را فراخوانی میکنیم. اینجا از switch case برای تشخیص دادن دستورات کاربر استفاده شده است. پس از انکه تابع مورد نظر فراخوانی شد، پاسخ از سمت سرور به کاربر ارسال میشود.  

```cpp
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
```

### کلاس `Client`:  
در این کلاس، مانند کلاس سرور، تابع `()build` نقش اساسی در client.cpp دارد(به علت طولانی بودن تابع، آن را قرار نمیدهیم و بخش های مهم آن را توضیح میدهیم).  
در ابتدای این تابع، `()connectServer` فراخوانی میشود که سعی میکند به سرور متصل شود. در صورتی که متصل نشد، خطایی چاپ میکنیم و به کاربر اطلاع میدهیم که ارتباط صورت نگرفته است. سپس در یک حلقه بی نهایت میرویم و در آنجا شرط های مختلف برای نشان دادن منوی اصلی و منوی کاربر را مشخص میکنیم.

```cpp
class Client
{
public:
    Client();
    void build();
    int connectServer(int port);
private:
    readJson data;
};
```

تابع `()connectServer` به صورت زیر است:

```cpp
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
```
تابع مهم بعدی در `client.cpp`، تابع `()show_list` میباشد. در این تابع، منوی اولیه به کاربر نشون داده میشود. وقتی کلاینت بالا می‌آید، این منو نمایش داده میشود و کاربر میتواند ثبت نام یا لاگین کند و یا از برنامه خارج شود. با زدن هریک از کامندها ، وارد تابع مربوط به خودش میشود.  

```cpp
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
```
منوی بعدی، منویی است که کلاینت بعد از لاگین کردن مشاهده میکند. در این منو اطلاعاتی از قبیل نشان دادن اطلاعات یوزر، نشان دادن اطلاعات دیگر کاربران برای ادمین، نشان دادن اتاق های هتل، رزرو و کنسل کردن اتاق، شبیه سازی گذشت زمان، ویرایش اطلاعات کاربر، رفتن از اتاق هتل، ویرایش اتاق هتل و خروج از برنامه نمایش داده میشود. سپس کاربر میتواند از بین این کامندها یکی را انتخاب کند و وارد تابع مربوط به خودش بشود.  

```cpp
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
        if (!isNumberBetween0And9(choice_num))
        {
            std::cout << ERR503 << std::endl;
            continue;               
        }
        else
        {
            if (stoi(choice_num) == 5)
            {
                if (user_admin != "user")
                    return "error403";
                return "menu/5/" + std::to_string(id);
            }
            return action_sentences(stoi(choice_num),id, user_admin);
        }
    }
    return "";
}
```

تابع مهم بعدی، تابع `()action_sentences` است که به علت طولانی بودن، از قرار دادن آن خودداری میکنیم و فقط منطق آن را توضیح میدهیم. در این بخش، یک switch case داریم و به ازای تمامی کامندها، بررسی میشود که ورودی های گرفته شده از کاربر به چه فرمتی باشد. همچنین در این بخش، استرینگی که به سرور فرستاده میشود نیز تنظیم میگردد. از طرف دیگر، هندل کردن ورودی های درست از کاربر نیز در این بخش انجام میشود.  

### کلاس `User`:

کلاس `User` حاوی اطلاعات یک کاربر در هتل است. توابعی که در بخش public قرار دارند، اطلاعات کاربر را فراخوانی میکنند یا این اطلاعات را تغییر میدهند. این اطلاعات شامل موارد زیر میباشد:

- `id`: شناسه کاربر
- `user`: نام کاربری
- `password`: رمز عبور
- `purse`: کیف پول
- `phoneNumber`: شماره تلفن
- `address`: آدرس

```cpp
class User
{
public:
    User(int id_, std::string user_, std::string password_, std::string purse_, std::string phoneNumber_, std::string address_);
    
    std::string getname();
    std::string getpassword();
    std::string getphone();
    std::string getaddress();
    std::string getpurse();
    
    void setpassword(std::string password_);
    void setaddress(std::string address_);
    void setphoneNumber(std::string phoneNumber_);
    void setpurse(std::string purse_);
    void cash_back(int cash);
    
    int getid();
private:
    int id;
    std::string user;
    std::string password;
    std::string purse;
    std::string phoneNumber;
    std::string address;
};
```

### کلاس `Admin`: 
در این کلاس اطلاعات ادمین نگه داری میشود. برای ادمین کلاس جدا از یوزر در نظر گرفتیم زیرا برای مواقعی که فقط با یوزر یا فقط با ادمین کار داریم، این کلاس ها را به صورت جداگانه میتوانیم استفاده کنیم. توابعی که در این کلاس استفاده شده است، مانند کلاس یوزر، شامل توابعی برای دسترسی به فیلد های پرایوت کلاس میباشد و تابع `()setpassword` برای تغییر دادن رمز استفاده میگردد. کلاس ادمین شامل این اطلاعات است:

- `id`: شناسه کاربر
- `user`: نام کاربری
- `password`: رمز عبور

```cpp
class Admin
{
public:
    Admin(int id_, std::string user_, std::string password_);

    std::string getname();
    std::string getpassword();

    int getid();
    
    void setpassword(std::string password_);
private:
    int id;
    std::string user;
    std::string password;
};
```

### کلاس `Rooms`:
این کلاس همانطور که از اسمش مشخص است، اطلاعات مربوط به اتاق ها را نگه میدارد. فیلد های پرایوت این کلاس، شامل ویژگی های هر اتاق است که در زیر آمده است. از طرف دیگر در این بخش یک وکتور تعریف شده است که اطلاعات مربوط به کاربرانی که در هر اتاق اقامت میکنند را نگه میدارد. توابع این کلاس، اطلاعات اتاق را فراخوانی میکنند یا این اطلاعات را تغییر میدهند. این اطلاعات شامل موارد زیر میباشد:

- `number`: شماره اتاق
- `status`: وضعیت اتاق
- `price`: قیمت اتاق به ازای هر نفر
- `maxCapacity`: ظرفیت حداکثر اتاق
- `capacity`: ظرفیت فعلی اتاق
- `userInRooms`: اطلاعات کاربرانی که در اتاق اقامت میکنند

```cpp
struct userInRoom
{
    int id;
    int numOfBeds;
    std::string reserveDate;
    std::string checkoutDate;
};

class Rooms
{
public:
    Rooms(std::string number_, int status_, int price_, int maxCapacity_, int capacity_, std::vector<userInRoom> userInRooms_);
    std::string getnum();
    
    int getstatus();
    int getprice();
    int getmax_capacity();
    int getcapacity();
    int get_numOfBeds(int index);

    std::vector<userInRoom> getusers();
    
    void set_price(int price_);
    void set_maxcap(int maxCapacity_);
    void set_capacity(int capacity_);
    void set_userInRooms(std::vector<userInRoom> userInRooms_);
    void set_status(int status_);
    void add_user(int id_, std::string numOfBeds_, std::string reserveDate_, std::string checkoutDate_);
    void change_capacity(int reduce_by);
    void del_reservation(int index);
    void set_numOfBeds(int index, int numOfBeds_);

private:
    std::string number;
    int status;
    int price;
    int maxCapacity;
    int capacity;
    std::vector<userInRoom> userInRooms;
};
```
