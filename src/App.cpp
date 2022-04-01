#include "App.hpp"

// set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
    0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
    0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
};

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

App::App()
{
    Engine::Log("Object Made");
}
App::~App()
{
    Engine::Log("Object Destroyed");
}

void App::Run()
{
    if (appState == AppState::ON)
        Engine::FatalError("App already running.");

    Engine::Init();

    unsigned int windowFlags = 0;

    // windowFlags |= Engine::WindowFlags::FULLSCREEN;

    // windowFlags |= Engine::WindowFlags::BORDERLESS;

    window.Create("Engine", 800, 600, windowFlags);

    Load();

    appState = AppState::ON;

    Loop();
}

void App::Load()
{
    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader program
    lightingShader.Compile("assets/shaders/2.2.basic_lighting.vs", "assets/shaders/2.2.basic_lighting.fs");
    lightingShader.AddAttribute("aPos");
    lightingShader.AddAttribute("aNormal");
    lightingShader.Link();

    lightCubeShader.Compile("assets/shaders/2.2.light_cube.vs", "assets/shaders/2.2.light_cube.fs");
    lightCubeShader.AddAttribute("aPos");
    lightCubeShader.Link();

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // wireframe
    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    // fill
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // start timer
    previousTime = high_resolution_clock::now();
}

void App::Loop()
{
    while (appState == AppState::ON)
    {
        currentTime = high_resolution_clock::now();
        deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - previousTime).count() / 1000000000.0;
        previousTime = currentTime;

        Update();
        Draw();
        // Get SDL to swap our buffer
        window.SwapBuffer();
        LateUpdate();
        FixedUpdate(deltaTime);
        InputUpdate();
    }
}
void App::Update()
{
    if (inputManager.isKeyPressed(SDLK_w))
    {
        camera.ProcessKeyboard(Engine::Camera_Movement::FORWARD, deltaTime);
    }

    if (inputManager.isKeyPressed(SDLK_s))
    {
        camera.ProcessKeyboard(Engine::Camera_Movement::BACKWARD, deltaTime);
    }

    if (inputManager.isKeyPressed(SDLK_a))
    {
        camera.ProcessKeyboard(Engine::Camera_Movement::LEFT, deltaTime);
    }

    if (inputManager.isKeyPressed(SDLK_d))
    {
        camera.ProcessKeyboard(Engine::Camera_Movement::RIGHT, deltaTime);
        Engine::Log("Right");
    }
}
void App::Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

    // be sure to activate shader when setting uniforms/drawing objects
    lightingShader.Use();
    lightingShader.SetVec3("objectColor", 1.0f, 0.5f, 0.31f);
    lightingShader.SetVec3("lightColor",  1.0f, 1.0f, 1.0f);
    lightingShader.SetVec3("lightPos", lightPos);
    lightingShader.SetVec3("viewPos", camera.Position);

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)window.GetScreenWidth() / (float)window.GetScreenHeight(), 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    lightingShader.SetMat4("projection", projection);
    lightingShader.SetMat4("view", view);

    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    lightingShader.SetMat4("model", model);

    // render the cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lightingShader.UnUse();


    // also draw the lamp object
    lightCubeShader.Use();
    lightCubeShader.SetMat4("projection", projection);
    lightCubeShader.SetMat4("view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    lightCubeShader.SetMat4("model", model);

    glBindVertexArray(lightCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    lightCubeShader.UnUse();
}

void App::LateUpdate() {}
void App::FixedUpdate(float dt) {}
void App::InputUpdate()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            appState = AppState::OFF;
            break;
        case SDL_MOUSEMOTION:
            camera.ProcessMouseMovement(
                event.motion.xrel,
                -event.motion.yrel);
            break;
        case SDL_KEYUP:
            inputManager.releasedKey(event.key.keysym.sym);
            break;
        case SDL_KEYDOWN:
            inputManager.pressKey(event.key.keysym.sym);
            break;
        }
    }
}