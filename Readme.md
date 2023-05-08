# Как использовать написанное? (Для баратанчиков)

### Получение экзепляров объектов:
```C++
// Создание Event(std::string msg) и User(std::string msg) зарезервировано для пересылки сообщений между клиентом и сервером

// Создать экземпляр события - по имени, дате, времени. 
Event temp("name", "date", "time");

// Создать экземпляр юзера - по логину и паролю
User temp("login", "password");

// Экземпляр базы данных для клиента (содержит только События)
DataBase_Client& main = DataBase_Client::get_instance();

// Экземпляр базы данных для сервера (содержит с Событиями и Юзерами)
DataBase_Server& main = DataBase_Server::get_instance();

```

### Методы обращения с таблицей Events:

```C++
// create_table_event(sqlite::database* db) - прописана в конструкторе базы базы данных и создает таблицу в ней

// Далее вроде очевидные методы с таблицей Событий
void               add_event(const Event& event)                  const;
Event              find_event_by_name(const std::string& name)    const;
void               remove_event_by_name(const std::string& name)  const;

void               print_all_events()               const;          // Print list of std::string names
std::vector<Event> get_all_events()                 const;
void               update_event(const Event& event) const;
```


### Методы обращения с таблицей Users:
```C++
// create_table_users(sqlite::database* db) - прописана в конструкторе базы базы данных и создает таблицу в ней

void               add_user(const User& user)                       const;
User               find_user_by_login(const std::string& login)     const;
void               remove_user_by_login(const std::string& login)   const;

void               print_all_users()                                const;      // Print list of std::string names
std::vector<User>  get_all_users()                                  const;

bool               verify_user(const User& user)                    const;
void               update_user(const User& user)                    const;  
```


How to use it in project?

```C++
// DataBase for Client (has only Events table):
DataBase_Client& base = DataBase_Client::get_instance();

// DataBase for Server (has Events and Users tables):
DataBase_Server& base = DataBase_Server::get_instance();
```
