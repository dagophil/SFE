add_executable(snake snake.cxx)
target_link_libraries(snake
	sfe
	${SFML_LIBRARIES}
)
copydir(snake img)
