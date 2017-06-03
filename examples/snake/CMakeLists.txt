globfiles(snake_SRC . .cxx)
globfiles(snake_HEADERS . .hxx)

add_executable(snake ${snake_SRC} ${snake_HEADERS})
target_link_libraries(snake
    sfe
)
copydir(snake img)
