#pragma once

#include "include/GL/glew.h"
#include "include/GLFW/glfw3.h"

#include "ChunkManager.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Shader.h"

#include "Camera.h"
enum class GameState{ RUN, QUIT };
static int window_width, window_height;
static bool windowFocused;


class Game
{
public:
	Game();
	Game(int window_width, int widow_height, const char* window_title);
	~Game();
	void Init(int width, int height, const char* window_title);
	
	void Run();
	
	void ManageInput();
	void ManageMouseInput();
private:
	GLFWwindow* _window;
	GameState _gameState;
	Camera _cam;

	ChunkManager _chunkManager;

	float mouseSpeed = 0.5f;
	float moveSpeed = 100.0f;
	Texture txt;
	Shader shader;
	VertexArray vao;
	VertexBuffer vb;
	IndexBuffer ib;
};
