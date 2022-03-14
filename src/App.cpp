#include "App.hpp"

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

    previousTime = std::chrono::high_resolution_clock::now();

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
  glEnable(GL_DEPTH_TEST);

  // build and compile our shader program
  // ------------------------------------
  shader.Compile("assets/shaders/6.3.transform.vs","assets/shaders/6.3.transform.fs");
  shader.AddAttribute("aPos");
  shader.AddAttribute("aTexCoord");
  shader.Link();
  



  // unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // color
  /*
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  */
  // texture coord
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // load texture 1
  texture1 = Engine::LoadPNGToGLTexture("assets/textures/container.png", GL_RGBA, GL_RGBA);
  // load texture 2
  texture2 = Engine::LoadPNGToGLTexture("assets/textures/awesomeface.png", GL_RGBA, GL_RGBA);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  glBindVertexArray(0);

  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
}

void App::Loop()
{
  while (appState == AppState::ON)
  {
    currentTime = std::chrono::high_resolution_clock::now();
    deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - previousTime).count() / 1000000000.0;
    previousTime = currentTime;
    Engine::Log(std::to_string(deltaTime));

    Update();
    Draw();
    // Get SDL to swap our buffer
    window.SwapBuffer();
    LateUpdate();
    FixedUpdate(0.0f);
    InputUpdate();
  }
}
void App::Update() {}
void App::Draw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // bind textures on corresponding texture units
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture1.id);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture2.id);

  // be sure to activate the shader before any calls to glUniform
  shader.Use();

  glUniform1i(glGetUniformLocation(shader.GetProgramID(), "texture1"), 0);
  glUniform1i(glGetUniformLocation(shader.GetProgramID(), "texture2"), 1);

  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);

  projection = glm::perspective(glm::radians(45.0f), (float)window.GetScreenWidth()/(float)window.GetScreenHeight(), 0.1f, 100.0f);
  view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));

  glUniformMatrix4fv(glGetUniformLocation(shader.GetProgramID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
  glUniformMatrix4fv(glGetUniformLocation(shader.GetProgramID(), "view"), 1, GL_FALSE, glm::value_ptr(view));

  //render Boxes
  glBindVertexArray(VAO);

  for (unsigned int i = 0; i < 10; i++)
  {

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, cubePositions[i]);
    float angle = 20.0f * i;
    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    glUniformMatrix4fv(glGetUniformLocation(shader.GetProgramID(), "model"), 1, GL_FALSE, glm::value_ptr(model));

    glDrawArrays(GL_TRIANGLES, 0, 36);

  }

  shader.UnUse();
}

void App::LateUpdate() {}
void App::FixedUpdate(float _delta_time) {}
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
      break;
    case SDL_KEYUP:
      break;
    case SDL_KEYDOWN:
      break;
    }
  }
}