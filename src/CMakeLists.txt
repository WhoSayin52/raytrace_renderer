target_sources(
	${EXE} PRIVATE
	core.hpp
	win32main.hpp win32main.cpp

	draw/draw.hpp draw/draw.cpp
	logger/logger.hpp logger/logger.cpp
)
