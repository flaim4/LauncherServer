cmake_minimum_required(VERSION 3.10)

project(LauncherServer)

set(CMAKE_CXX_STANDARD 17)

link_directories(${BOOST_LIB} ${MYSQL_LIB})

add_executable(LauncherServer
    ${CMAKE_SOURCE_DIR}/src/LauncherServer.cpp
    ${CMAKE_SOURCE_DIR}/src/handlers/register_handler.cpp
    ${CMAKE_SOURCE_DIR}/src/handlers/login_handler.cpp
    ${CMAKE_SOURCE_DIR}/src/handlers/logout_handler.cpp
    ${CMAKE_SOURCE_DIR}/src/utility/flaim.cpp
)

target_link_libraries(LauncherServer PRIVATE
    ${BOOST_LIB}/libboost_json.a
    ${BOOST_LIB}/libboost_system.a
    ${BOOST_LIB}/libboost_thread.a

)

find_library(RESULT libmysqlcppconn.so.10.9.2.0 PATHS ${MYSQL_LIB})

include_directories(${BOOST_INCLUDE} ${MYSQL_INCLUDE})

set(CMAKE_EXE_LINKER_FLAGS "-static")
