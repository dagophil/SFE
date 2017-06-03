globfiles(sfetests_SRC . .cxx)
globfiles(sfetests_HEADERS . .hxx)

add_executable(sfetests ${sfetests_SRC} ${sfetests_HEADERS})
target_link_libraries(sfetests
    sfe
)
copyfile(sfetests disk.png)
