#include "Game.h"

#include <cmath>
#include <iostream>
#include <math.h>

#include "include/GLFW/glfw3.h"
#include "utils/Timer.h"


void WindowSizeCallback(GLFWwindow* window, int width, int height);
void WindowFocusCallback(GLFWwindow* window, int focused);


Game::Game()
{
	_gameState = GameState::RUN;
}

Game::Game(int window_width, int widow_height, const char* window_title)
{
	Init(window_width, widow_height, window_title);
}


Game::~Game()
{
	glfwTerminate();
}

void Game::Init(int width, int height, const char* window_title)
{
	_gameState = GameState::RUN;
	window_width = width;
	window_height = height;

	_cam.pos = glm::vec3(19200,19200,50);
	_cam.verticalAngle = 0.0f;
	_cam.horizontalAngle = 3.9f;

	glewExperimental = true;
	if(!glfwInit())
		std::cout<<"[ERROR]  GLFW could not initialize."<<std::endl;

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_window = glfwCreateWindow(window_width, window_height, window_title, glfwGetPrimaryMonitor(), NULL);
	if(_window == NULL)
		std::cout<<"[ERROR]  Window could not be created."<<std::endl;

	glfwMakeContextCurrent(_window);
	glewExperimental = true;
	if(glewInit() != GLEW_OK)
		std::cout<<"[ERROR]  GLEW could not initialize."<<std::endl;

	glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetWindowSizeCallback(_window, WindowSizeCallback);

	glfwSetWindowFocusCallback(_window, WindowFocusCallback);
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Enable depth test (to stop triangles in back from redrawing some in the front)
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	//Face Culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glDisable(GL_CULL_FACE);

	glEnable( GL_BLEND );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glfwSwapInterval(0);

	glfwSetCursorPos(_window, (int)(window_width/2), (int)(window_height/2));
	int tmpwidth, tmpheight;
	float xscale, yscale;
	glfwGetWindowSize(_window, &tmpwidth, &tmpheight);
	glfwGetWindowContentScale(_window, &xscale, &yscale);
	glViewport(1, 0, tmpwidth * xscale, tmpheight * yscale);
}

void Game::Run()
{
	std::cout<<"width: "<<window_width<<std::endl;
	std::cout<<"height: "<<window_height<<std::endl;
	glClearColor(0.0f, 0.6f, 0.86f, 0.0f);

	shader.Init(2);
	shader.LoadShader(GL_VERTEX_SHADER, "assets/VertexShader.glsl");
	shader.LoadShader(GL_FRAGMENT_SHADER, "assets/FragmentShader.glsl");
	shader.GenProgram();

	txt.Init(1);
	txt.Load("assets/blocks.png");

	{
		Timer cg("Total Chunk Gen time");
		_chunkManager.Init(20);
		_chunkManager.GenerateChunks(glm::floor(_cam.pos));
	}
	while(_gameState != GameState::QUIT)
	{
		Timer whilelp("Rest of Game Loop");
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		whilelp.pause();
		Timer dt("Draw Time");
		_chunkManager.DrawChunks(&_cam, &shader, (float)window_width/(float)window_height, _cam.pos, _cam.dir);
		dt.end();
		Timer ut("Update Time");
		_chunkManager.UpdateChunks(_cam.pos);
		ut.end();
		whilelp.resume();
		glfwSwapBuffers(_window);
		ManageInput();
	}
}

void Game::ManageInput()
{
	static float prevtime;
	float deltaTime = glfwGetTime() - prevtime;
	prevtime = glfwGetTime();
	glfwPollEvents();
	ManageMouseInput();
	if(glfwGetKey(_window, GLFW_KEY_Q) || glfwWindowShouldClose(_window))
		_gameState = GameState::QUIT;
	if(glfwGetKey(_window, GLFW_KEY_W))
		_cam.pos += glm::normalize(_cam.dir * glm::vec3(1,1,0)) * moveSpeed * deltaTime;
	if(glfwGetKey(_window, GLFW_KEY_A))
		_cam.pos -= glm::normalize(_cam.right * glm::vec3(1,1,0)) * moveSpeed * deltaTime;
	if(glfwGetKey(_window, GLFW_KEY_S))
		_cam.pos -= glm::normalize(_cam.dir * glm::vec3(1,1,0)) * moveSpeed * deltaTime;
	if(glfwGetKey(_window, GLFW_KEY_D))
		_cam.pos += glm::normalize(_cam.right * glm::vec3(1,1,0)) * moveSpeed * deltaTime;
	if(glfwGetKey(_window, GLFW_KEY_SPACE))
		_cam.pos.z += moveSpeed * deltaTime;
	if(glfwGetKey(_window, GLFW_KEY_LEFT_CONTROL))
		_cam.pos.z -= moveSpeed * deltaTime;
	if(glfwGetKey(_window, GLFW_KEY_0))
		moveSpeed += 0.1;
	if(glfwGetKey(_window, GLFW_KEY_9))
		moveSpeed -= 0.1;
	if(glfwGetKey(_window, GLFW_KEY_B))
		_chunkManager.RefreshGPU();

}


void WindowSizeCallback(GLFWwindow* window, int width, int height)
{
	float xscale, yscale;
	glfwGetWindowContentScale(window, &xscale, &yscale);
	glViewport(1, 0, width * xscale, height * yscale);
	window_width = width;
	window_height = height;
}

void WindowFocusCallback(GLFWwindow* window, int focused)
{
	windowFocused = focused;
}


void Game::ManageMouseInput()
{
	static bool holdMouse = true;
	if(glfwGetKey(_window, GLFW_KEY_L))
		holdMouse = !holdMouse;
	if(!windowFocused || !holdMouse)
		return;


	static float prevtime;
	float deltaTime = glfwGetTime() - prevtime;
	prevtime = glfwGetTime();
	double xpos, ypos;
	glfwGetCursorPos(_window, &xpos, &ypos);
	glfwSetCursorPos(_window, (int)(window_width/2), (int)(window_height/2));

	_cam.horizontalAngle += mouseSpeed * deltaTime * ( window_width/2.0f  - xpos ) * (-1.0f);

	float v_delta = mouseSpeed * deltaTime * ( window_height/2.0f - ypos ) * (+1.0f);
	if(!(std::abs(_cam.verticalAngle + v_delta) > 1.57f))
		_cam.verticalAngle  += v_delta;

	float va = _cam.verticalAngle;
	float ha = _cam.horizontalAngle;
	_cam.dir = glm::normalize(glm::vec3(
	cos(va) * sin(ha),
	cos(va) * cos(ha),
	sin(va)));

	_cam.right = glm::normalize(glm::vec3(
	cos(ha)*cos(va),
	cos(va)*cos(ha+ 3.14/2.0),
	0));

	_cam.up = glm::cross( _cam.right, _cam.dir ) * glm::vec3(1,1,1);
}
