cmake_minimum_required(VERSION 3.10)

project(LauncherServer)

set(CMAKE_CXX_STANDARD 17)


add_executable(LauncherServer
    ${CMAKE_SOURCE_DIR}/src/LauncherServer.cpp
    ${CMAKE_SOURCE_DIR}/src/handlers/register_handler.cpp
    ${CMAKE_SOURCE_DIR}/src/handlers/login_handler.cpp
    ${CMAKE_SOURCE_DIR}/src/handlers/logout_handler.cpp
    ${CMAKE_SOURCE_DIR}/src/utility/flaim.cpp
)

target_link_libraries(LauncherServer
    ${BOOST_LIB}/libboost_json.a
    ${BOOST_LIB}/libboost_system.a
    ${BOOST_LIB}/libboost_thread.a
)

include_directories(${BOOST_INCLUDE})

set(CMAKE_EXE_LINKER_FLAGS "-static")

find_package(Boost 1.77 REQUIRED COMPONENTS json system thread)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(LauncherServer ${Boost_LIBRARIES})
endif()
