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

# Define a copy makro.
macro(copyfile tar filename)
	add_custom_command(TARGET ${tar} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   ${CMAKE_CURRENT_SOURCE_DIR}/${filename} ${CMAKE_CURRENT_BINARY_DIR}/${filename})
endmacro()

#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCHECKEVENTTYPE")

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(examples)
