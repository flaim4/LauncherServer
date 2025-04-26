cmake_minimum_required(VERSION 3.10)

project(LauncherServer)

set(CMAKE_CXX_STANDARD 17)

set(BOOST_ROOT "/boost")

find_package(Boost 1.87 REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(LauncherServer
    ${CMAKE_SOURCE_DIR}/src/LauncherServer.cpp
    ${CMAKE_SOURCE_DIR}/src/handlers/register_handler.cpp
    ${CMAKE_SOURCE_DIR}/src/handlers/login_handler.cpp
    ${CMAKE_SOURCE_DIR}/src/handlers/logout_handler.cpp
    ${CMAKE_SOURCE_DIR}/src/utility/flaim.cpp
)

target_link_libraries(LauncherServer
    Boost::Boost
    ws2_32
)

set(CMAKE_EXE_LINKER_FLAGS "-static")
