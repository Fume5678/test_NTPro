cmake_minimum_required(VERSION 3.11)
set(this client)
project(${this})

if(WIN32)
    set (BOOST_ROOT "C:/boost_1_80_0/") # Путь к библиотеке Boost
endif()

set(CMAKE_CXX_STANDARD 17)
set (Boost_NO_SYSTEM_PATHS ON)
set (Boost_USE_MULTITHREADED ON)
set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_STATIC_RUNTIME OFF)
set (BOOST_ALL_DYN_LINK OFF)

find_package (Boost REQUIRED COMPONENTS program_options thread REQUIRED) 

FILE(GLOB_RECURSE SOURCES "src/*.cpp")
FILE(GLOB_RECURSE HEADERS "include/*.h")

if (Boost_FOUND)
    include_directories (SYSTEM ${Boost_INCLUDE_DIR}) # подключаем заголовочные файлы
    add_executable (${this} ${SOURCES} ${HEADERS}) # Создаем исполняемый файл для нашего проекта
    target_link_libraries (${this} ${Boost_LIBRARIES}) # подключаем библиотеку
    target_include_directories(${this} PRIVATE "./external/")
endif ()