target_sources(
	${EXE} PRIVATE
	core.hpp
	win32main.hpp win32main.cpp

	logger/logger.hpp logger/logger.cpp
	math/math.hpp math/vector.hpp/ math/vector.cpp
	renderer/renderer.hpp renderer/renderer.cpp renderer/scene.hpp
	utility/utility.hpp
)
