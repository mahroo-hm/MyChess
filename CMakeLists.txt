cmake_minimum_required(VERSION 3.1.0)
project(MyChess)

set(CMAKE_CXX_STANDARD 11)

set(SRC_DIRECTORY source)
set(INCLUDE_DIRECTORY header)

set(SOURCES
    ${SRC_DIRECTORY}/board.cpp
    ${SRC_DIRECTORY}/critical.cpp
    ${SRC_DIRECTORY}/essentials.cpp
    ${SRC_DIRECTORY}/game.cpp
    ${SRC_DIRECTORY}/piece.cpp)


include_directories(${INCLUDE_DIRECTORY})

add_executable(${PROJECT_NAME}
    ${SRC_DIRECTORY}/main.cpp
    ${SOURCES})
    
target_link_libraries(${PROJECT_NAME}
    sfml-system
    sfml-window
    sfml-graphics)
    
    