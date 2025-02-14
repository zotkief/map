cmake_minimum_required(VERSION 3.10)

# Nazwa projektu
project(map)

# Ustaw standard C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Dodaj wszystkie pliki .cpp z katalogu src
file(GLOB SRC_FILES src/*.cpp)

# Utwórz plik wykonywalny
add_executable(${PROJECT_NAME} main.cpp ${SRC_FILES})

# Dodaj katalog z nagłówkami (include)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Znajdź bibliotekę GDAL
find_package(GDAL REQUIRED)

# Podłącz GDAL
target_link_libraries(${PROJECT_NAME} PRIVATE ${GDAL_LIBRARIES})

# Znajdź bibliotekę OpenCV
find_package(OpenCV REQUIRED)

# Podłącz OpenCV (poprawienie składni, użycie jednej wersji)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})
