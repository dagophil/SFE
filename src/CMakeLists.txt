globfiles(sfe_SRC . .cxx)
globfiles(sfe_HEADERS ${SFE_INCLUDE_DIR} .hxx)

add_library(sfe ${LIBRARY_TYPE} ${sfe_SRC} ${sfe_HEADERS})

target_link_libraries(sfe
    ${Boost_FILESYSTEM_LIBRARY}
    ${SFML_LIBRARIES}
)
