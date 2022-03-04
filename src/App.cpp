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

  window.Create("Engine", 600, 600, windowFlags);
  
  Load();

  appState = AppState::ON;

  Loop();
}

void App::Load()
{
  /*
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
  */

  shader.Compile("assets/shaders/4.2.texture.vs", "assets/shaders/4.2.texture.fs");
  shader.AddAttribute("ourColor");
  shader.AddAttribute("TexCoord");
  shader.Link();

  float vertices[] = {
    //Position                Color                 Texture Coordanates

    0.5f, 0.5f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
    -0.5, -0.5, 0.0,     0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
    -0.5f, 0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
   // 0.0f, 0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   0.0f, 1.0f

    // All these need to be floats

  };

  unsigned int indices[] = {
    0, 1, 3,  //Triangle 1
    1, 2, 3,  //Triangle 2
  };

  //VBO, VAO
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

//position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
//color
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
//texture
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

//Load tex 1

  texture1 = Engine::LoadPNGToGLTexture("assets/textures/container.png", GL_RGBA, GL_RGBA);

//Load tex 2

  texture2 = Engine::LoadPNGToGLTexture("assets/textures/awesomeface.png", GL_RGBA, GL_RGBA);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);
/*
  glUniform1i(glGetUniformLocation(shader.GetProgramID(), "texture1"), 0);
  glUniform1i(glGetUniformLocation(shader.GetProgramID(), "texture2"), 1);
*/
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

  //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  // bind textures to corrisponding units
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture1.id);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture2.id);
  
  //Be sure to activate the shader program before calling any uniforms
  /*
  glUseProgram(shaderProgram);
  */

  shader.Use();

  glUniform1i(glGetUniformLocation(shader.GetProgramID(), "texture1"), 0);
  glUniform1i(glGetUniformLocation(shader.GetProgramID(), "texture2"), 1);

  //Update shader uniform
  //double timeValue = SDL_GetTicks() / 1000;
  //float redValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);
  //float greenValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);
  //float blueValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);
  //int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
  //glUniform4f(vertexColorLocation, redValue, greenValue, blueValue, 1.0f);

  //shader.SetVec4("ourColor", glm::vec4(redValue, greenValue, blueValue, 1.0f));

  //Render Triangle

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  shader.UnUse();

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