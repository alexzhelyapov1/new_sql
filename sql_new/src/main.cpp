#include <iostream>
#include <sqlite_modern_cpp.h>
#include "sql_new.h"
using namespace  sqlite;
using namespace std;

int main() 
{
   // DataBase& b = DataBase::get_instance();
   try 
   {
      // Table_Events table;
      // table.create_table_event(b.get_db());
      // spdlog::info("Table created");   

      // Event ev1("1", "1", "1");
      // table.add_event(ev1);
      // table.print_all_events();

      DataBase_Server& main = DataBase_Server::get_instance();
      User e1("1", "1");
      User e2("2", "2");
      main.add_user(e1);
      main.add_user(e2);
      main.print_all_users();
      User new_user("1", "4");

      spdlog::info("List of creds:\n");
      for (auto& item : main.get_all_users())
      {
         std::cout << item << std::endl;
      }

      main.update_user_password(new_user);

      spdlog::info("List of creds:\n");
      for (auto& item : main.get_all_users())
      {
         std::cout << item << std::endl;
      }

   }
   catch (const sqlite::sqlite_exception& e) {
      spdlog::error("{}: {} during {}", e.get_code(), e.what(), e.get_sql());
   }

   system("rm main.db");

}


int test()
{
   DataBase& b = DataBase::get_instance();
   try {
      Table_Events table;
      table.create_table_event(b.get_db());

      Event ev1("Name1", "date 1", "time 1");
      table.add_event(ev1);
   }
   catch (const sqlite::sqlite_exception& e) {
      spdlog::error("{}: {} during {}", e.get_code(), e.what(), e.get_sql());
   }
   return 0;
}