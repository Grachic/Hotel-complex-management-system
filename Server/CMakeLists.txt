cmake_minimum_required(VERSION 3.20)
project(Server)

set(Qt5_DIR "C:/Qt/Qt5.12.12/5.12.12/mingw73_64/lib/cmake/Qt5")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets Network Sql REQUIRED)

add_executable(Server main.cpp log/output log/header/logger.h log/source/logger.cpp
        network/source/server.cpp
        network/header/server.h
        database/model/header/usermodel.h database/model/source/usermodel.cpp
        database/header/database.h database/source/database.cpp
        constants/constants.h
        database/model/header/hotelroommodel.h database/model/source/hotelroommodel.cpp
        database/model/header/employeemodel.h database/model/source/employeemodel.cpp
        database/model/header/bookeddatemodel.h database/model/source/bookeddatemodel.cpp
        )
target_link_libraries(Server Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network Qt5::Sql)