#include "App.hpp"
#include "math.h"

App::App(){
  Engine::Log("Object Made");
}
App::~App(){
  Engine::Log("Object Destroyed");
}

void App::Run(){
  if (appState == AppState::ON)
    Engine::FatalError("App already running.");
  
  Engine::Init();

  unsigned int windowFlags = 0;

  //windowFlags |= Engine::WindowFlags::FULLSCREEN;

  //windowFlags |= Engine::WindowFlags::BORDERLESS;

  window.Create("Engine", 800, 600, windowFlags);
  
  Load();

  appState = AppState::ON;

  Loop();
}

void App::Load()
{

  //Build and compile shaders

  //Vertex shader
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  //Check for shader compile errors
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if(!success)
  {

    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "Error::Shader::Vertex::CompileFailure\n" << infoLog << std::endl;

  }

  //Fragment Shader
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  //Check for compile error
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if(!success)
  {

  glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "Error::Shader::FRAGMENT::CompileFailure\n" << infoLog << std::endl;

  }

  //Link shaders
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  //Check for link errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if(!success)
  {

  glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "Error::Shader::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  float vertices[] = {
    -0.5f, -0.5f, 0.0f, //left
    0.5f, -0.5f, 0.0f, //right
    0.0f, 0.5f, 0.0f //top
  };

  //VBO, VAO
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

}
void App::Loop()
{
  while(appState == AppState::ON)
  {
    Update();
    Draw();
    // Get SDL to swap our buffer
    window.SwapBuffer();
    LateUpdate();
    FixedUpdate(0.0f);
    InputUpdate();
  }
}
void App::Update(){}
void App::Draw()
{

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  //Be sure to activate the shader program before calling any uniforms

  glUseProgram(shaderProgram);

  //Update shader uniform
  double timeValue = SDL_GetTicks() / 1000;
  float redValue = 0.0;
  float greenValue = static_cast<float>(sin(timeValue) / 2.0 + 5.0);
  float blueValue = 0.0;
  int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
  glUniform4f(vertexColorLocation, redValue, greenValue, blueValue, 1.0f);

  //Render Triangle

  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindVertexArray(0);

}
void App::LateUpdate(){}
void App::FixedUpdate(float _delta_time){}
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