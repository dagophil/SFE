cmake_minimum_required(VERSION 2.8)

project(sfelibrary)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/config)
include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(Boost 1.6.0 COMPONENTS REQUIRED system filesystem)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

 # SFML
 find_package(SFML REQUIRED system window graphics)
 include_directories(${SFML_INCLUDE_DIR})
 #add_definitions(-DSFML_DYNAMIC)

add_subdirectory(src)
add_subdirectory(test)
