add_executable(snake snake.cxx)
target_link_libraries(snake
	sfe
	${SFML_LIBRARIES}
)
copyfile(snake camel_bg.jpg)
copyfile(snake snake_head.png)
copyfile(snake snake_body.png)
