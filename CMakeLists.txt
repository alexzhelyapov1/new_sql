cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
project("interactive_event_map")


file(GLOB SQL_NEW "sql_new/src/*")
add_executable(InteractiveEventMap_sql ${SQL_NEW})
target_include_directories(InteractiveEventMap_sql PUBLIC "sql_new/include")


find_package(spdlog CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)


target_link_libraries(InteractiveEventMap_sql PUBLIC unofficial::sqlite3::sqlite3 spdlog::spdlog spdlog::spdlog_header_only)

find_path(SQLITE_MODERN_CPP_INCLUDE_DIRS "sqlite_modern_cpp.h")
target_include_directories(InteractiveEventMap_sql PRIVATE ${SQLITE_MODERN_CPP_INCLUDE_DIRS})