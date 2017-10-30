#include "App.h"

App::App(AppConfig config)
{
    this->config = config;

    if (!InitSdl())
    {
        Logger::LogError("cant init sdl");
    };

    if (!InitRenderer())
    {
        Logger::LogError("cant init renderer");
    };

    if (!InitOpengl())
    {
        Logger::LogError("cant init gl");
    };

    ImGui_ImplSdlGL3_Init(renderer->GetWindow());
}

bool App::InitSdl()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        return false;
    }

    return true;
}

bool App::InitOpengl()
{
    if (!gladLoadGL())
    {
        return false;
    }

    //	glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_PROGRAM_POINT_SIZE);

    return true;
}

bool App::InitRenderer()
{
    renderer = new Renderer(config);

    return true;
}

void App::Run()
{
    CreateModel(algorithmIndex);
    while (appState == AppState::RUN)
    {
        glClearColor(config.red / 255, config.green / 255, config.blue / 255, 1.f);
        Update();
        Draw();
    }

    ImGui_ImplSdlGL3_Shutdown();
    SDL_GL_DeleteContext(renderer->GetRendererContext());
    SDL_DestroyWindow(renderer->GetWindow());
    SDL_Quit();
}

void App::Update()
{
    ProcessInput();
    // Fruchterman_Reingold();

    if (graphModel != nullptr && skip > 4)
    {   
        graphModel->Update();
        skip = 0;
    }

    skip++;
    camera->Update();
}

void App::Draw()
{
    renderer->BeginDraw();
    auto cameraPos = camera->GetPosition();

    // edges.isVisible = config.showEdge;
    // nodes.Draw(camera->GetProjectionMatrix(), camera->GetViewMatrix(), cameraPos);
    // edges.Draw(camera->GetProjectionMatrix(), camera->GetViewMatrix(), cameraPos);

    if (graphModel != nullptr)
    {
        graphModel->Draw(camera->GetProjectionMatrix(), camera->GetViewMatrix(), cameraPos);
    }

    RenderGui();
    renderer->EndDraw();
}

void App::ProcessInput()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            StopApp();
            break;
        case SDL_KEYDOWN:
            inputManager.pressKey(event.key.keysym.sym);
            break;
        case SDL_KEYUP:
            inputManager.releaseKey(event.key.keysym.sym);
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_RIGHT)
            {
                inputManager.pressMouseKey(SDL_BUTTON_RIGHT);
            }
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                inputManager.pressMouseKey(SDL_BUTTON_LEFT);
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_RIGHT)
            {
                inputManager.releaseMouseKey(SDL_BUTTON_RIGHT);
            }
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                inputManager.releaseMouseKey(SDL_BUTTON_LEFT);
            }
            break;
        case SDL_MOUSEMOTION:
            inputManager.SetMousePosition(event.motion.x, event.motion.y);
            std::stringstream ss;
            ss << "X: " << inputManager.GetMousePosition().x << " Y: " << inputManager.GetMousePosition().y;
            SDL_SetWindowTitle(renderer->GetWindow(), ss.str().c_str());
            break;
        }
    }

    if (inputManager.isKeyPressed(SDLK_ESCAPE))
    {
        StopApp();
    }

    if (inputManager.isKeyPressed(SDLK_SPACE))
    {
        camera->Reset();
    }

    if (inputManager.isKeyPressed(SDLK_w))
    {
        camera->Forward();
    }

    if (inputManager.isKeyPressed(SDLK_s))
    {
        camera->Back();
    }

    if (inputManager.isKeyPressed(SDLK_a))
    {
        camera->Left();
    }

    if (inputManager.isKeyPressed(SDLK_d))
    {
        camera->Right();
    }

    if (inputManager.isKeyPressed(SDLK_r))
    {
        camera->Up();
    }

    if (inputManager.isKeyPressed(SDLK_f))
    {
        camera->Down();
    }

    if (inputManager.isKeyPressed(SDLK_LEFT))
    {
        camera->RotateLeft();
    }

    if (inputManager.isKeyPressed(SDLK_RIGHT))
    {
        camera->RotateRight();
    }

    if (inputManager.isKeyPressed(SDLK_UP))
    {
        camera->RotateUp();
    }

    if (inputManager.isKeyPressed(SDLK_DOWN))
    {
        camera->RotateDown();
    }

    if (inputManager.isMouseKeyPressed(SDL_BUTTON_RIGHT))
    {
        camera->MouseMovement(inputManager.GetMousePosition());
        inputManager.ResetMousePosition(inputManager.GetMousePosition().x, inputManager.GetMousePosition().y);
        //SDL_ShowSimpleMessageBox( 0, "Mouse", "Left button was pressed!", renderer->GetWindow() );
    }

    if (inputManager.isMouseKeyPressed(SDL_BUTTON_LEFT))
    {
        auto view = camera->GetViewMatrix();
        auto projection = camera->GetProjectionMatrix();
        auto mousePosition = inputManager.GetMousePosition();

        glm::vec3 start = glm::unProject(glm::vec3(mousePosition.x, this->config.height - mousePosition.y, 0),
                                         view, projection, glm::vec4(0, 0, this->config.width, this->config.height));
        glm::vec3 end = glm::unProject(glm::vec3(mousePosition.x, this->config.height - mousePosition.y, 1),
                                       view, projection, glm::vec4(0, 0, this->config.width, this->config.height));

        auto origin = camera->GetPosition();
        auto direction = glm::normalize(end - start);
        int aaa = 0;
    }

    ImGui_ImplSdlGL3_ProcessEvent(&event);
}

void App::StopApp()
{
    appState = AppState::EXIT;
}

void App::AddCamera(Camera *camera)
{
    this->camera = camera;
}

void App::SetAppState(AppState state)
{
    this->appState = state;
}

void App::SetNodesCount(int count)
{
    nodeCount = count;
}

void App::RenderGui()
{
    ImGui_ImplSdlGL3_NewFrame(renderer->GetWindow());

    if (config.showGui)
    {

        string nodesText = "nodes count: " + to_string(nodeCount);
        ImGui::Text(nodesText.c_str());
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);

        ImGui::Begin("Camera settings");
        {
            ImGui::Text("Camera position");
            if (ImGui::InputFloat("Camera position x", &camera->cameraPosition.x, 0.0f, 1000000.0f) || ImGui::InputFloat("Camera position y", &camera->cameraPosition.y, 0.0f, 1000000.0f) || ImGui::InputFloat("Camera position z", &camera->cameraPosition.z, 0.0f, 1000000.0f))
            {
                camera->MakePosition();
            }

            ImGui::Text("Camera rotation");
            if (ImGui::InputFloat("Camera rotation x", &camera->cameraRotation.x, 0.0f, 1000000.0f) || ImGui::InputFloat("Camera rotation y", &camera->cameraRotation.y, 0.0f, 1000000.0f) || ImGui::InputFloat("Camera rotation 5z", &camera->cameraRotation.z, 0.0f, 1000000.0f))
            {
                camera->MakeRotation();
                camera->Back();
            }
        }
        ImGui::End();

        ImGui::Begin("System settings");
        {
            ImGui::SliderFloat("red", &config.red, 0.0f, 255.0f);
            ImGui::SliderFloat("green", &config.green, 0.0f, 255.0f);
            ImGui::SliderFloat("blue", &config.blue, 0.0f, 255.0f);
        }
        ImGui::End();
    }

    ImGui::Begin("Algorithms");
    {
        ImGui::Checkbox("Show edge", &config.showEdge);
        ImGui::Checkbox("Update", &config.isUpdateOn);

        if (ImGui::RadioButton("F-R gpu improved", &algorithmIndex, 3))
        {
            CreateModel(algorithmIndex);
        }
        ImGui::SameLine();
        
        if (ImGui::RadioButton("F-R gpu", &algorithmIndex, 0))
        {
            CreateModel(algorithmIndex);
        }
        ImGui::SameLine();

        if(ImGui::RadioButton("F-R cpu", &algorithmIndex, 1)){
            CreateModel(algorithmIndex);
        };
        
        if(ImGui::RadioButton("Random gpu", &algorithmIndex, 2)){
            CreateModel(algorithmIndex);
        };
    }

    ImGui::End();

    if (graphModel != nullptr)
    {
        graphModel->DrawGui();
    }

    ImGui::Render();
}

void App::CreateModel(int algorithmIndex)
{
    if(graphModel != nullptr){
        graphModel->Clear();
        graphModel = nullptr;
    }
    graphModel = ModelCreator::GetModelByType(modelData, &config, algorithmIndex);
}