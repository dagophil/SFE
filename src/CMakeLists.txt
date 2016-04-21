add_library(sfe widget.cxx screen.cxx resource_manager.cxx input.cxx)
target_link_libraries(sfe
	${Boost_FILESYSTEM_LIBRARY}
)
