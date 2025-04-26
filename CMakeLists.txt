cmake_minimum_required(VERSION 3.10)

project(LauncherServer)

find_package(Boost 1.74 REQUIRED json)


set(CMAKE_CXX_STANDARD 17)


include_directories(
    ${CMAKE_SOURCE_DIR}/../app_src/boost_1_87_0
    ${CMAKE_SOURCE_DIR}/../app_src/json
)


set(SOURCES
    ../app_src/src/LauncherServer.cpp
    ../app_src/src/handlers/register_handler.cpp
    ../app_src/src/handlers/login_handler.cpp
    ../app_src/src/handlers/logout_handler.cpp
    ../app_src/src/utility/flaim.cpp
)

add_executable(LauncherServer ${SOURCES})

target_link_libraries(LauncherServer
    ../app_src/libboost_json.a
    ../app_src/libboost_system.a
    ../app_src/libboost_thread.a
    pthread
)
