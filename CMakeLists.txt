cmake_minimum_required(VERSION 3.2)

project(Minecraft)

set(CMAKE_BUILD_TYPE Debug)
set(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

add_definitions(-DENABLE_LOG)

add_subdirectory(deps/glfw)

add_executable(${PROJECT_NAME} 
	${CMAKE_SOURCE_DIR}/deps/glew/glew.c

	${CMAKE_SOURCE_DIR}/src/Chunk.cpp
	${CMAKE_SOURCE_DIR}/src/ChunkManager.cpp
	${CMAKE_SOURCE_DIR}/src/Game.cpp
	${CMAKE_SOURCE_DIR}/src/IndexBuffer.cpp
	${CMAKE_SOURCE_DIR}/src/Shader.cpp
	${CMAKE_SOURCE_DIR}/src/Texture.cpp
	${CMAKE_SOURCE_DIR}/src/VertexArray.cpp
	${CMAKE_SOURCE_DIR}/src/VertexBuffer.cpp
	${CMAKE_SOURCE_DIR}/src/main.cpp
	${CMAKE_SOURCE_DIR}/src/utils/File.cpp
	${CMAKE_SOURCE_DIR}/src/utils/Timer.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src/include)

if(WIN32)
target_link_libraries(${PROJECT_NAME} opengl32)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
else()
target_link_libraries(${PROJECT_NAME} GL)
endif()

target_link_libraries(${PROJECT_NAME} glfw)
