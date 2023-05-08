//----------------------------------------------------------------
//  This part realises lib of classes which interacts with SQL databases
//  Designed for project Interactive Map (group project for 4sem in MIPT)
//
//  @copyright AlexZ
//----------------------------------------------------------------


#include "sql_new.h"


// ---------------------------------------------------------------------------------------------------------------------
// Constructions for communication with server
// 
// Operator std::string convert object to string, parsing by \n
// Construcotr of msg - construct the object from msg by parsing msg by \n
// ---------------------------------------------------------------------------------------------------------------------



User::operator std::string() const 
{
    std::string str = std::to_string(id_) + "\n";
    str += login_ + "\n";
    str += password_ + "\n";
    str += std::to_string(last_edit_time_) + "\n";
    return str;
}



User::User(std::string msg)
{
    std::vector<std::string> data_vector;
    std::string temp;
    std::stringstream input_stream(msg);

    for (int i = 0; i < 4; i++)
    {
        std::getline(input_stream, temp);
        data_vector.push_back(temp);
        // spdlog::info("Readed part = '{}'", temp);
    }

    id_ = std::atoi(data_vector[0].c_str());
    login_ = data_vector[1];
    password_ = data_vector[2];
    last_edit_time_ = std::atol(data_vector[3].c_str());

}



Event::operator std::string() const 
{
    std::string str = std::to_string(id_) + "\n";
    str += name_ + "\n";
    str += info_ + "\n";
    str += address_ + "\n";
    str += date_ + "\n";
    str += time_ + "\n";
    str += owner_ + "\n";
    str += std::to_string(last_edit_time_) + "\n";
    return str;
}



Event::Event(std::string msg)
{
    std::vector<std::string> data_vector;
    std::string temp;
    std::stringstream input_stream(msg);

    for (int i = 0; i < 8; i++)
    {
        std::getline(input_stream, temp);
        data_vector.push_back(temp);
    }


    id_             = std::atoi(data_vector[0].c_str());
    name_           = data_vector[1];
    info_           = data_vector[2];
    address_        = data_vector[3];
    date_           = data_vector[4];
    time_           = data_vector[5];
    owner_          = data_vector[6];
    last_edit_time_ = std::atol(data_vector[7].c_str());
}



// ---------------------------------------------------------------------------------------------------------------------
// std::cout operators <<
// ---------------------------------------------------------------------------------------------------------------------



std::ostream& operator<< (std::ostream &out, const User &User)
{
    out << User.id_ << "    " << User.login_ << "    " << User.password_ << "    " << User.last_edit_time_;

    return out;
}



std::ostream& operator<< (std::ostream &out, const Event &event)
{
    out << event.id_ << "    " << event.name_ << "    " << event.info_ << "    " << event.address_
                     << "    " << event.date_ << "    " << event.time_ << "    " << event.owner_ << "    " << event.last_edit_time_;

    return out;
}



// ---------------------------------------------------------------------------------------------------------------------
// Initialize DataBase
// ---------------------------------------------------------------------------------------------------------------------



DataBase::DataBase()
{
    db_ = new sqlite::database("main.db");
}



DataBase::~DataBase()
{
    try
    {
        delete db_;
        spdlog::info("Database closed");
    }
    catch(std::exception& e)
    {
        spdlog::error("Can't close Database!");
    }
}



// ---------------------------------------------------------------------------------------------------------------------
// Table_Events
// ---------------------------------------------------------------------------------------------------------------------



void Table_Events::create_table_event(sqlite::database* db)
{
    db_ = db;

    *db_ << "create table if not exists EVENTS ("
                "ID integer primary key autoincrement NOT NULL,"
                "NAME                      TEXT       NOT NULL, "
                "INFO                      TEXT               , "
                "ADDRESS                   TEXT               , "
                "DATE                      TEXT               , "
                "TIME                      TEXT               , "
                "OWNER                     TEXT               , "
                "LAST_EDIT_TIME            INT                );";


    spdlog::info("Table EVENTS created");
}



void Table_Events::add_event(const Event& event) const
{
    // *db_ << u"INSERT into EVENTS (NAME,INFO,ADDRESS,DATE,TIME,OWNER,LAST_EDIT_TIME) values (?,?,?,?,?,?,?);"
    if (!db_) spdlog::critical("Zero db in add event");

    *db_ << u"INSERT INTO EVENTS (NAME,INFO,ADDRESS,DATE,TIME,OWNER,LAST_EDIT_TIME) VALUES (?,?,?,?,?,?,?);"
        << event.get_name()
        << event.get_info()
        << event.get_address()
        << event.get_date()
        << event.get_time()
        << event.get_owner()
        << event.get_last_edit_time();
}



Event Table_Events::find_event_by_name(const std::string& name) const
{
    std::vector<Event> events_vec;
    *db_ << u"SELECT * FROM EVENTS WHERE NAME=?" << name >> [&](int id, std::string name, std::string info, std::string address, 
            std::string date, std::string time, std::string owner, size_t last_edit_time)
            {
                Event temp(id, name, info, address, date, time, owner, last_edit_time);
                events_vec.push_back(temp);
            };

    if (events_vec.size() > 1)
    {
        spdlog::critical("Found more than 1 event by name");
        return events_vec[0];
    }
    else if (events_vec.size() == 1)
    {   
        return events_vec[0];
    }
    else
    {
        spdlog::warn("No event found by name = '{}'", name);
        return Event();
    }
}



void Table_Events::remove_event_by_name(const std::string& name) const
{
    *db_ << u"DELETE FROM EVENTS WHERE NAME=?;" << name;
}



void Table_Events::print_all_events() const
{
    std::cout << "Print all events:\n";
    
    *db_ << "SELECT NAME FROM EVENTS;" >> [&](std::string name) 
    {
        std::cout << name << std::endl;
    };
}



std::vector<Event> Table_Events::get_all_events() const
{
    std::cout << "Print all events:\n";
    std::vector<Event> events_vec;
    
    *db_ << "SELECT * FROM EVENTS;" >> [&](int id, std::string name, std::string info, std::string address, 
            std::string date, std::string time, std::string owner, size_t last_edit_time) 
    {
        Event temp(id, name, info, address, date, time, owner, last_edit_time);
        events_vec.push_back(temp);
    };

    return events_vec;
}



void Table_Events::update_event(const Event& event) const
{
    remove_event_by_name(event.get_name());
    add_event(event);
}



void Table_Events::rename_event(const std::string& old_name, const std::string& new_name) const
{
    *db_ << u"UPDATE EVENTS SET NAME=? WHERE NAME=?" << new_name << old_name;
}



// ---------------------------------------------------------------------------------------------------------------------
// Table_Users
// ---------------------------------------------------------------------------------------------------------------------



void Table_Users::create_table_users(sqlite::database* db)
{
    db_ = db;

    *db_ << "create table if not exists USERS ("
                "ID integer primary key autoincrement NOT NULL,"
                "LOGIN                     TEXT       NOT NULL, "
                "PASSWORD                  TEXT       NOT NULL, "
                "LAST_EDIT_TIME            INT                );";


    spdlog::info("Table USERS created");    
}



void Table_Users::add_user(const User& user) const
{
    if (!db_) spdlog::critical("Zero db in add user");

    *db_ << u"INSERT INTO USERS (LOGIN,PASSWORD,LAST_EDIT_TIME) VALUES (?,?,?);"
        << user.get_login()
        << sha256(user.get_password())
        << user.get_last_edit_time();
}



User Table_Users::find_user_by_login(const std::string& login) const
{
    std::vector<User> users_vec;
    *db_ << u"SELECT * FROM USERS WHERE LOGIN=?" << login >> [&](int id, std::string login, std::string password,
                                                                                                size_t last_edit_time)
            {
                User temp(id, login, password, last_edit_time);
                users_vec.push_back(temp);
            };

    if (users_vec.size() > 1)
    {
        spdlog::critical("Found more than 1 user by login");
        return User("", "");
    }
    else if (users_vec.size() == 1)
    {   
        return users_vec[0];
    }
    else
    {
        spdlog::warn("No event found by name = '{}'", login);
        return User("", "");
    }
}



void Table_Users::remove_user_by_login(const std::string& login)  const
{
    *db_ << u"DELETE FROM USERS WHERE LOGIN=?;" << login;
}



void Table_Users::print_all_users() const
{
    std::cout << "Print all users:\n";
    
    *db_ << "SELECT LOGIN FROM USERS;" >> [&](std::string login) 
    {
        std::cout << login << std::endl;
    };
}



std::vector<User> Table_Users::get_all_users() const
{
    std::cout << "Print all Users:\n";
    std::vector<User> users_vec;
    
    *db_ << "SELECT * FROM USERS;" >> [&](int id, std::string login, std::string password, size_t last_edit_time) 
    {
        User temp(id, login, password, last_edit_time);
        users_vec.push_back(temp);
    };

    return users_vec;
}



bool Table_Users::verify_user(const User& user) const
{
    std::vector<User> users_vec;
    *db_ << u"SELECT * FROM USERS WHERE LOGIN=? and PASSWORD=?" << user.get_login() << sha256(user.get_password()) >> 
                                            [&](int id, std::string login, std::string password, size_t last_edit_time)
            {
                User temp(id, login, password, last_edit_time);
                users_vec.push_back(temp);
            };

    if (users_vec.size() > 1)
    {
        spdlog::critical("Found more than 1 user by login");
        return true;
    }
    else if (users_vec.size() == 1)
    {   
        return true;
    }
    else
    {
        spdlog::warn("No event found by name = '{}'", user.get_login());
        return false;
    }
}



void Table_Users::update_user_password(const User& user) const
{
    *db_ << u"UPDATE USERS SET PASSWORD=? WHERE LOGIN=?" << sha256(user.get_password()) << user.get_login();
}



void Table_Users::update_user_password(const std::string& user_name, const std::string& new_password) const
{
    User temp(user_name, new_password);
    update_user_password(temp);
}



std::string Table_Users::sha256(const std::string& data) const
{
    return data + "psw";
}