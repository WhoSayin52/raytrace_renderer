target_sources(
	${EXE} PRIVATE
	win32main.hpp win32main.cpp

	core/core.hpp core/core.cpp
	logger/logger.hpp logger/logger.cpp
	renderer/renderer.hpp renderer/renderer.cpp
	utility/utility.hpp
)
