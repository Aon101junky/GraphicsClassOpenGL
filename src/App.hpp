#include <iostream>
#include <SDL.h>
#include <map>
#include <math.h>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Engine/Engine.hpp"
#include "Engine/Debug.hpp"
#include "Engine/Window.hpp"
#include "Engine/Shader.hpp"
#include "Engine/Camera.hpp"
#include "Engine/Model.hpp"
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

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
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
    void RenderText(Engine::Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color);

    AppState appState = AppState::OFF;

    Engine::Window window;

    Engine::Shader asteroidShader;
    Engine::Shader planetShader;
    Engine::Shader textShader;

    Engine::Model rock;
    Engine::Model planet;

    Engine::Camera camera = Engine::Camera(glm::vec3(0.0f, 0.0f, 300.0f));

    Engine::InputManager inputManager;

    high_resolution_clock::time_point currentTime;
    high_resolution_clock::time_point previousTime;

    float deltaTime;

    bool mouseLock = true;

    std::map<GLchar, Character> Characters;

    FT_Library ft;

    FT_Face face;

    unsigned int VAOText, VBOText, VAO, VBO, buffer;

    glm::mat4* modelMatrices;

    unsigned int amount = 1000;
};