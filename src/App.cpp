#include "App.hpp"

// set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
float vertices[] = {
  // positions           // texture coords
  -0.5f, -0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      0.5f,  0.5f, -0.5f,
      0.5f,  0.5f, -0.5f,
  -0.5f,  0.5f, -0.5f,
  -0.5f, -0.5f, -0.5f,

  -0.5f, -0.5f,  0.5f,
      0.5f, -0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
  -0.5f,  0.5f,  0.5f,
  -0.5f, -0.5f,  0.5f,

  -0.5f,  0.5f,  0.5f,
  -0.5f,  0.5f, -0.5f,
  -0.5f, -0.5f, -0.5f,
  -0.5f, -0.5f, -0.5f,
  -0.5f, -0.5f,  0.5f,
  -0.5f,  0.5f,  0.5f,

      0.5f,  0.5f,  0.5f,
      0.5f,  0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      0.5f, -0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,

  -0.5f, -0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      0.5f, -0.5f,  0.5f,
      0.5f, -0.5f,  0.5f,
  -0.5f, -0.5f,  0.5f,
  -0.5f, -0.5f, -0.5f,

  -0.5f,  0.5f, -0.5f,
      0.5f,  0.5f, -0.5f,
      0.5f,  0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
  -0.5f,  0.5f,  0.5f,
  -0.5f,  0.5f, -0.5f,
};

// world space positions of our cubes
glm::vec3 cubePositions[] = {
  glm::vec3( 0.0f,  0.0f,  0.0f),
  glm::vec3( 2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3( 2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3( 1.3f, -2.0f, -2.5f),
  glm::vec3( 1.5f,  2.0f, -2.5f),
  glm::vec3( 1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};

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
  lightingShader.Compile("assets/shaders/1.colors.vs", "assets/shaders/1.colors.fs");
  lightingShader.AddAttribute("aPos");
  lightingShader.Link();

  lightCubeShader.Compile("assets/shaders/1.light_cube.vs", "assets/shaders/1.light_cube.fs");
  lightCubeShader.AddAttribute("aPos");
  lightCubeShader.Link();

  //unsigned int VBO, VAO;
  glGenVertexArrays(1, &cubeVAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(cubeVAO);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glGenVertexArrays(1, &lightCubeVAO);
  glBindVertexArray(lightCubeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0); 

  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  glBindVertexArray(0);

  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);

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
  }
}
void App::Draw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

  // activate shader
  lightingShader.Use();
  lightingShader.SetVec3("objectColor", 1.0f, 0.5f, 0.31f);
  lightingShader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);

  // create transformations
  glm::mat4 view          = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
  glm::mat4 projection    = glm::mat4(1.0f);
  projection = glm::perspective(glm::radians(camera.Zoom), (float)window.GetScreenWidth() / (float)window.GetScreenHeight(), 0.1f, 100.0f);
  view       = camera.GetViewMatrix();
  // pass transformation matrices to the shader
  lightingShader.SetMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
  lightingShader.SetMat4("view", view);

  // render boxes
  glBindVertexArray(cubeVAO);

      glm::mat4 model = glm::mat4(1.0f);
      lightingShader.SetMat4("model", model);

      glDrawArrays(GL_TRIANGLES, 0, 36);

  lightingShader.UnUse();

  lightCubeShader.Use();
  lightCubeShader.SetMat4("projection", projection);
  lightCubeShader.SetMat4("view", view);

  model = glm::mat4(1.0f);
  model = glm::translate(model, lightPos);
  model = glm::scale(model, glm::vec3(0.2f));
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
        -event.motion.yrel
      );
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