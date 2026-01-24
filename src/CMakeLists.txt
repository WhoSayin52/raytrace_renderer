target_sources(
	${EXE} PRIVATE
	core.hpp
	win32main.hpp win32main.cpp

	renderer/renderer.hpp renderer/renderer.cpp
	logger/logger.hpp logger/logger.cpp
)
