cmake_minimum_required(VERSION 3.10)

#project name
project(map)

#c++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#linking src files
file(GLOB SRC_FILES src/*.cpp)

add_executable(${PROJECT_NAME} main.cpp ${SRC_FILES})

#linking include folder
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

find_package(GDAL REQUIRED)

#linking GDAL
target_link_libraries(${PROJECT_NAME} PRIVATE ${GDAL_LIBRARIES})

find_package(OpenCV REQUIRED)

#linking OpenCV
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})

#setting lib directories
set(SQLITE3_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/sqlite3/include")
set(SQLITE3_LIB_DIR "${CMAKE_SOURCE_DIR}/lib/sqlite3/src")

include_directories(${SQLITE3_INCLUDE_DIR})

link_directories(${SQLITE3_LIB_DIR})

find_package(CURL REQUIRED)

target_link_libraries(map PRIVATE CURL::libcurl)


#selectig binary
set(SQLITE3_LIB "sqlite3")

# Dodaj SQLite3 do Twojego projektu
target_link_libraries(${PROJECT_NAME} PRIVATE ${SQLITE3_LIB})
