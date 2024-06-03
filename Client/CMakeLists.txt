cmake_minimum_required(VERSION 3.20)
project(Client)

set(Qt5_DIR "C:/Qt/Qt5.12.12/5.12.12/mingw73_64/lib/cmake/Qt5")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets Network REQUIRED)

file(GLOB SOURCE_FILES
        "log/header/logger.h" "log/source/logger.cpp"
        "widgets/source/*.cpp" "widgets/header/*.h" "widgets/ui/*.ui"
        "network/header/*.h" "network/source/*.cpp"
        )

file(GLOB EXTRA_FILES
        "resources/extra libraries/EToastr/EToastr.cpp" "resources/extra libraries/EToastr/EWidget.cpp"
        "resources/extra libraries/EToastr/EToastr.h" "resources/extra libraries/EToastr/EWidget.h"
        "resources/extra libraries/EToastr/Ui/toastr.ui" "resources/extra libraries/EToastr/Ui/image.qrc"
        )

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

add_executable(Client main.cpp resource.qrc ${SOURCE_FILES} ${EXTRA_FILES})
target_link_libraries(Client Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network)