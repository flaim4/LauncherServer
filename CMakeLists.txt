cmake_minimum_required(VERSION 3.10)

project(LauncherServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Boost REQUIRED COMPONENTS system thread json)

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/json
)

set(SOURCES
    src/LauncherServer.cpp
    src/handlers/register_handler.cpp
    src/handlers/login_handler.cpp
    src/handlers/logout_handler.cpp
    src/utility/flaim.cpp
)

add_executable(LauncherServer ${SOURCES})

target_link_libraries(LauncherServer
    Boost::system
    Boost::thread
    Boost::json
    pthread
)
