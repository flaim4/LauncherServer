cmake_minimum_required(VERSION 3.10)

project(LauncherServer)

include_directories(${CMAKE_SOURCE_DIR}/boost_1_87_0)
include_directories(${CMAKE_SOURCE_DIR}/json)

add_executable(LauncherServer
        ${CMAKE_SOURCE_DIR}/src/LauncherServer.cpp
        ${CMAKE_SOURCE_DIR}/src/handlers/register_handler.cpp
        ${CMAKE_SOURCE_DIR}/src/handlers/login_handler.cpp
        ${CMAKE_SOURCE_DIR}/src/handlers/logout_handler.cpp
        ${CMAKE_SOURCE_DIR}/src/utility/flaim.cpp
)

target_link_libraries(LauncherServer
        ${CMAKE_SOURCE_DIR}/libboost_json.a
        ${CMAKE_SOURCE_DIR}/libboost_system.a
        ${CMAKE_SOURCE_DIR}/libboost_thread.a
        ws2_32
)

set(CMAKE_EXE_LINKER_FLAGS "-static")