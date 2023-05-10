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
      main.add_event_bug();
      // main.add_event(void add_event_bug());

      main.remove_event_by_name("5");
      Event ev1(0, "5", "5", "5", "5", "5", "", 1683760471);
      main.add_event(ev1);
      for (const auto& item : main.get_all_events())
      {
         std::cout << item << std::endl;
      }

   }
   catch (const sqlite::sqlite_exception& e) {
        spdlog::error("{}: {} during {}\nFile = '{}', function = '{}', line = '{}'", e.get_code(), e.what(), e.get_sql(),
                                                __FILE__, __FUNCTION__, __LINE__);
    }

}