#include <iostream>
#include <SDL.h>

#include "Engine/Engine.hpp"
#include "Engine/Debug.hpp"
#include "Engine/Window.hpp"

enum AppState
{
	ON,
	OFF
};

class App
{
public:
	App();
	~App();

	void Run();

private:
	void Load();
	void Loop();
	void Update();
	void Draw();
	void LateUpdate();
	void FixedUpdate(float _delta_time);
	void InputUpdate();

	AppState appState = AppState::OFF;

	Engine::Window window;

	// Move this to external

	unsigned int shaderProgram;
	unsigned int VBO, VAO;

	const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

    const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

};