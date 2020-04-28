cmake_minimum_required(VERSION 3.0.0)
project(EpollServer)

set(Qt5_DIR /home/whl/Qt5.9.3/5.9.3/gcc_64/lib/cmake/Qt5)

add_compile_options(-std=c++11 -fpermissive)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
FILE(GLOB SRC_FILES "EpollServer/*.cpp")
FILE(GLOB HEADER_FILES "EpollServer/*.h")
FILE(GLOB UI_SRC_FILES "*.cpp")
FILE(GLOB UI_HEADER_FILES "*.h")
FILE(GLOB UI_FILES "*.ui")

# qt5_wrap_ui(WRAP_FILES ${UI_FILES})
# set(RCC_FILES rcc.qrc)
find_package(Qt5 REQUIRED Widgets Core Gui)

add_executable(${PROJECT_NAME} ${UI_SRC_FILES} ${UI_HEADER_FILES} ${RCC_FILES} ${WRAP_FILES} ${SRC_FILES} ${HEADER_FILES})
target_link_libraries(${PROJECT_NAME} pthread)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core Qt5::Gui)