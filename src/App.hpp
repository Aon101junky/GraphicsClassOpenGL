#include <iostream>
#include <SDL.h>
#include <math.h>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Engine/Engine.hpp"
#include "Engine/Debug.hpp"
#include "Engine/Window.hpp"
#include "Engine/Shader.hpp"
#include "Engine/Camera.hpp"
#include "Engine/IOManager.hpp"
#include "Engine/InputManager.hpp"
#include "Engine/Data/GLTexture.hpp"

#ifdef __linux__
using namespace std::chrono::_V2;
#elif _WIN32
using namespace std::chrono;
#else
#endif

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
    void FixedUpdate(float dt);
    void InputUpdate();

    AppState appState = AppState::OFF;

    Engine::Window window;

    Engine::Shader lightingShader;

    Engine::Shader lightCubeShader;

    Engine::Camera camera = Engine::Camera(glm::vec3(0.0f, 0.0f, 3.0f));

    Engine::InputManager inputManager;

    // move out to external class
    unsigned int vertexShader;
    unsigned int shaderProgram;
    unsigned int VBO, cubeVAO, lightCubeVAO, EBO, lightVBO;

    Engine::GLTexture diffuseMap = {};
    Engine::GLTexture specularMap = {};

    high_resolution_clock::time_point currentTime;
    high_resolution_clock::time_point previousTime;

    float deltaTime;

    bool mouseLock = true;
};