cmake_minimum_required(VERSION 3.5)

project(sfelibrary)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/config)

# SFE includes
set(SFE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
include_directories(${SFE_INCLUDE_DIR})

# Boost
find_package(Boost 1.6.0 COMPONENTS REQUIRED system filesystem)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

# SFML
find_package(SFML REQUIRED system window graphics)
include_directories(${SFML_INCLUDE_DIR})
#add_definitions(-DSFML_DYNAMIC)

# Define a copy file makro.
macro(copyfile tar filename)
    add_custom_command(TARGET ${tar} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/${filename} ${CMAKE_CURRENT_BINARY_DIR}/${filename})
endmacro()

# Define a copy directory makro.
macro(copydir tar dirname)
    add_custom_command(TARGET ${tar} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/${dirname} ${CMAKE_CURRENT_BINARY_DIR}/${dirname})
endmacro()

# Define a macro to build file lists.
macro(globfiles tar dirname extension)
    file(GLOB ${tar}
        "${dirname}/*${extension}"
        "${dirname}/*/*${extension}"
        "${dirname}/*/*/*${extension}"
        "${dirname}/*/*/*/*${extension}"
        "${dirname}/*/*/*/*/*${extension}"
        "${dirname}/*/*/*/*/*/*${extension}"
        "${dirname}/*/*/*/*/*/*/*${extension}"
        "${dirname}/*/*/*/*/*/*/*/*${extension}")
endmacro()

# Check the event types in debug mode.
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DCHECKEVENTTYPE")

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(examples)
