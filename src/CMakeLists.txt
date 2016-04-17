add_library(sfe widget.cxx)
target_link_libraries(sfe
	${Boost_FILESYSTEM_LIBRARY}
)
