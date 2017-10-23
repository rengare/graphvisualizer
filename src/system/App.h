#ifndef APP_H
#define APP_H

#ifdef _WIN32
#define min(a, b) ((a < b) ? a : b)
#endif

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <vector>
#include <map>
#include <math.h>
#include <sstream>

#include "input/InputManager.h"
#include "../graphic/camera/Camera.h"
#include "../config/AppConfig.h"
#include "../graphic/Renderer.h"
#include "../graphic/GraphicsStructure.h"
#include "../helper/Logger.h"
#include "../helper/ModelCreator.h"
#include "../graphic/camera/Camera.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_sdl_gl3.h"

#include "../graphic/GraphicsStructure.h"

#include "../models/IModel.h"


#include "../models/FR/FRModel.h"
#include "../models/FR/FRModelCpu.h"

using namespace std;

enum class AppState
{
  RUN,
  EXIT
};

class App
{
public:
  App(AppConfig config);

  void Run();

  void Update();

  void Draw();

  void AddCamera(Camera *camera);
  void SetAppState(AppState state);
  void SetNodesCount(int count);

  IModel *graphModel = nullptr;
  ModelData* modelData = nullptr;

private:
  bool InitSdl();
  bool InitRenderer();

  bool InitOpengl();
  void ProcessInput();

  void StopApp();

  void RenderGui();

  void CreateModel(int algorithmIndex);

private:
  AppState appState;

  AppConfig config;

  InputManager inputManager{};
  Renderer *renderer;

  Camera *camera;
  glm::mat4 projectionMatrix;
  glm::mat4 viewMatrix;

  int nodeCount;
  int nodeIndex = 0;

  int currentNodeIteration = 0;
  int currentEdgeIteration = 0;

  float SPEED_DIVISOR = 800;
  float AREA_MULTIPLICATOR = 10000;
  float speed = 1;
  float area = 1000;
  float gravity = 10;
  int algorithmIndex = 3;
};

#endif