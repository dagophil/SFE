add_executable(sfetests sfetests.cxx)
target_link_libraries(sfetests
	sfe
	${SFML_LIBRARIES}
)
