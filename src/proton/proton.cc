//
// Created by Monsler on 04.07.2025.
//

#include "proton/proton.hh"

#include "SDL3/SDL_timer.h"
#include "box2d/box2d.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include "proton/logman.hh"
#include "proton/physics.hh"
#include "proton/resourcemanager.hh"

namespace Proton
{
Display::Display(const std::string &title, const int w, const int h)
    : pointerX(0), pointerY(0), windowWidth(w), windowHeight(h), imguiio(nullptr)
{
    Proton::LogNew(LogInfo::Info, "Initing display..");
    this->title = title;
    SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "wayland,x11,windows,android");
    SDL_SetHint(SDL_HINT_VIDEO_WAYLAND_SCALE_TO_DISPLAY, "1");
    SDL_SetHint(SDL_HINT_RENDER_LINE_METHOD, "1");
    SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight");

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        Log("Error initializing SDL: ", SDL_GetError());
        return;
    }

    TTF_Init();

    int window_flags = 0;
#ifdef __ANDROID__
    window_flags |= SDL_WINDOW_FULLSCREEN;
#else
    window_flags |= SDL_WINDOW_RESIZABLE;
#endif

    this->handle = SDL_CreateWindow(this->title.c_str(), w, h, window_flags);
    if (!this->handle)
    {
        Log("Error creating window: ", SDL_GetError());
        return;
    }

    this->render = SDL_CreateRenderer(this->handle, nullptr);
    if (!this->render)
    {
        Log("Error initializing display: ", SDL_GetError());
        return;
    }

    SDL_SetRenderVSync(this->render, 1);

    ResourceManager::getInstance().initAudioEngine();
    Physics::initPhysicsDevice(-9.8f);
    LogNew(LogInfo::Info, "Initializing is successful");

    // imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    this->imguiio = &ImGui::GetIO();
    (void)this->imguiio;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL3_InitForSDLRenderer(this->handle, this->render);
    ImGui_ImplSDLRenderer3_Init(this->render);

    this->isInit = true;
    this->currentScene = nullptr;
}

void Display::setScene(Scene *newScene)
{
    if (this->currentScene != nullptr)
    {
        delete currentScene;
    }

    this->currentScene = newScene;
}

void Display::startRendering()
{
    if (this->isInit)
    {
        this->renderStart();

        // on end
        delete this->currentScene;
        SDL_DestroyWindow(this->handle);
        SDL_DestroyRenderer(this->render);
        ResourceManager::getInstance().clearCache();
        TTF_Quit();
        SDL_Quit();
        Physics::destroyPhysicsDevice();
    }
    else
    {
        summonError();
    }
}

auto Display::getWindowHeight() const -> int { return this->windowHeight; }

auto Display::getWindowWidth() const -> int { return this->windowWidth; }

auto Display::getSurface() const -> SDL_Surface * { return SDL_GetWindowSurface(this->handle); }

auto Display::getRenderer() const -> SDL_Renderer * { return this->render; }

auto Display::getNativeWindow() const -> SDL_Window * { return this->handle; }

void Display::setTitle(const char *title) const { SDL_SetWindowTitle(this->handle, title); }

void Display::setIcon(const std::string &path) const
{
    SDL_Surface *icon = ResourceManager::getInstance().getIcon(path);
    SDL_SetWindowIcon(this->handle, icon);
}

void Display::setRenderScale(const double x, const double y) const
{
    SDL_SetRenderScale(this->render, static_cast<float>(x), static_cast<float>(y));
}

void Display::renderStart()
{
    SDL_Event e;
    bool isDone = false;

    Uint64 lastFrameTime = SDL_GetTicks();
    float deltaTime = 0.0f;

    Proton::LogNew(LogInfo::Info, "Render started!");

    const Uint64 perfFrequency = SDL_GetPerformanceFrequency();
    constexpr auto color_ok = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
    constexpr auto color_warn = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
    constexpr auto color_bad = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    constexpr auto color_info = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    while (!isDone)
    {
        const Uint64 eventMeterC_Start = SDL_GetPerformanceCounter();
        while (SDL_PollEvent(&e))
        {
            SDL_ConvertEventToRenderCoordinates(render, &e);
            ImGui_ImplSDL3_ProcessEvent(&e);

            switch (e.type)
            {
            case SDL_EVENT_QUIT:
            {
                isDone = true;
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_UP:
            {
                if (!this->currentScene)
                    break;
                if (this->imguiio->WantCaptureMouse)
                    break;
                Point mousePoint{e.button.x, e.button.y};
                this->currentScene->handleButtonClickEnd(mousePoint);
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            {
                if (!this->currentScene)
                    break;
                if (this->imguiio->WantCaptureMouse)
                    break;
                Point mousePoint{e.button.x, e.button.y};
                this->currentScene->mouseDown(mousePoint);
                this->currentScene->handleButtonClick(mousePoint);
                break;
            }
            case SDL_EVENT_MOUSE_MOTION:
            {
                if (!this->currentScene)
                    break;
                if (e.motion.state & SDL_BUTTON_LMASK)
                {
                    this->currentScene->handleMouseDrag(static_cast<int>(e.motion.x), static_cast<int>(e.motion.y));
                }
                break;
            }
            case SDL_EVENT_KEY_DOWN:
            {
                if (!this->currentScene)
                    break;
                if (this->imguiio->WantCaptureKeyboard)
                    break;
                if (e.key.key == SDLK_F12)
                    this->showDebug = !this->showDebug;
                this->currentScene->handleKeyDown(e);
                this->currentScene->keyPressed(e.key.key);
                break;
            }
            case SDL_EVENT_TEXT_INPUT:
            {
                if (!this->currentScene)
                    break;
                if (this->imguiio->WantCaptureKeyboard)
                    break;
                this->currentScene->handleTextInput(e);
                break;
            }
            default:
                break;
            }
        }
        const double eventMeterC_Time =
            (double)(SDL_GetPerformanceCounter() - eventMeterC_Start) * 1000.0 / (double)perfFrequency;

        if (!this->currentScene)
        {
            continue;
        }

        const auto currentTime = static_cast<float>(SDL_GetTicks());
        deltaTime = (currentTime - static_cast<float>(lastFrameTime)) / 1000.0f;
        lastFrameTime = static_cast<Uint64>(currentTime);

        const Uint64 physicMeterC_Start = SDL_GetPerformanceCounter();
        Physics::update(deltaTime);
        for (const PhysicsBody *body : currentScene->getPhysicsBodies())
        {
            const b2BodyId bodyId = body->getBody();
            if (Shape *shape = body->getUsedShape())
            {
                const auto [x, y] = b2Body_GetPosition(bodyId);
                const double angle = b2Rot_GetAngle(b2Body_GetRotation(bodyId));
                shape->setRotation(static_cast<float>(-(angle * (180 / M_PI))));

                const float pixelX = x * PIXELS_PER_METER;
                const float pixelY = -y * PIXELS_PER_METER;

                shape->setPosition(pixelX - shape->getW() / 2.0f, pixelY - shape->getH() / 2.0f);
            }
        }
        const double physicMeterC_Time =
            (double)(SDL_GetPerformanceCounter() - physicMeterC_Start) * 1000.0 / (double)perfFrequency;

        SDL_SetRenderDrawColor(this->render, this->currentScene->getBackgroundColor().getR(),
                               this->currentScene->getBackgroundColor().getG(),
                               this->currentScene->getBackgroundColor().getB(),
                               this->currentScene->getBackgroundColor().getA());

        float px, py;
        SDL_GetMouseState(&px, &py);
        SDL_RenderClear(this->render);

        if (Scene *nextScene = this->currentScene->update(deltaTime); nextScene != this->currentScene)
        {
            if (nextScene == nullptr)
            {
                isDone = true;
            }
            this->setScene(nextScene);
        }

        const Uint64 renderMeterC_Start = SDL_GetPerformanceCounter();
        if (this->currentScene)
        {
            this->currentScene->paint();
        }
        const double renderMeterC_Time =
            (double)(SDL_GetPerformanceCounter() - physicMeterC_Start) * 1000.0 / (double)perfFrequency;

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        if (showDebug)
        {
            ImGui::Begin("Proton2D Debugger");
            if (ImGui::CollapsingHeader("Performance"))
            {
                if (ImGui::BeginTable("stats", 2, ImGuiTableFlags_BordersInnerV))
                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("FPS");
                    ImGui::TableNextColumn();
                    ImGui::Text("%.1f", this->imguiio->Framerate);

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("Physic update");
                    ImGui::TableNextColumn();
                    ImGui::Text("%f ms", physicMeterC_Time);

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("Render update");
                    ImGui::TableNextColumn();
                    ImGui::Text("%f ms", renderMeterC_Time);

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("Event polling");
                    ImGui::TableNextColumn();
                    ImGui::Text("%f ms", eventMeterC_Time);

                    ImGui::EndTable();
                }
            }

            ImGui::End();
        }

        if (showDebug)
        {
            ImGui::Begin("Proton2D Engine Output");
            if (ImGui::Button("Clear"))
            {
                _protonLoggedVector.clear();
            }
            ImGui::SameLine();
            static bool auto_scroll = true;
            ImGui::Checkbox("Auto-scroll", &auto_scroll);

            ImGui::Separator();

            ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

            for (const auto &logged_item : _protonLoggedVector)
            {
                ImVec4 color;
                switch (logged_item.logLevel)
                {
                case Proton::LogInfo::Warn:
                    color = color_warn;
                    break;
                case Proton::LogInfo::Error:
                    color = color_bad;
                    break;
                case Proton::LogInfo::Info:
                default:
                    color = color_info;
                    break;
                }
                ImGui::TextColored(color, "%s", logged_item.message.c_str());
            }

            if (auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            {
                ImGui::SetScrollHereY(1.0f);
            }

            ImGui::EndChild();
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), this->render);
        SDL_RenderPresent(this->render);
    }
    SDL_GetWindowSize(this->handle, &this->windowWidth, &this->windowHeight);
}

void Display::summonError() { Log("Init display first!"); }
} // namespace Proton
