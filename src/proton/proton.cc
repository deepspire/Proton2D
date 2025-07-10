//
// Created by Monsler on 04.07.2025.
//

#include "proton/proton.hh"

#include "box2d/box2d.h"
#include "proton/logman.hh"
#include "proton/physics.hh"
#include "proton/resourcemanager.hh"

namespace Proton
{
Display::Display(const std::string &title, const int w, const int h)
    : pointerX(0), pointerY(0), windowWidth(w), windowHeight(h)
{
    this->title = title;
    SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "wayland,x11,windows,android");
    SDL_SetHint(SDL_HINT_VIDEO_WAYLAND_SCALE_TO_DISPLAY, "1");
    SDL_SetHint(SDL_HINT_RENDER_LINE_METHOD, "1");
    SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight");

    const int ret = SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    Log("SDL Video Driver: ", SDL_GetCurrentVideoDriver());
    if (ret < 0)
    {
        Log("Error initializing SDL: ", SDL_GetError());
        return;
    }

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
    /*if (!SDL_SetRenderLogicalPresentation(this->render, w, h, SDL_LOGICAL_PRESENTATION_LETTERBOX))
    {
      Log("Unable to set SDL_SetRenderLogicalPresentation", SDL_GetError());
    }*/

    Log("display init is successful");

    ResourceManager::getInstance().initAudioEngine();
    Physics::initPhysicsDevice(-9.8f);
    this->isInit = true;
    this->currentScene = nullptr;
}

void Display::setScene(Scene *newScene)
{
    if (this->currentScene != nullptr)
    {
        delete currentScene;
        Log("Deleting previous scene...");
    }

    this->currentScene = newScene;
    Log("bodies ", this->currentScene->getPhysicsBodies().size());
}

void Display::startRendering()
{
    if (this->isInit)
    {
        this->renderStart();
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

    while (!isDone)
    {
        while (SDL_PollEvent(&e))
        {
            SDL_ConvertEventToRenderCoordinates(render, &e);

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
                Point mousePoint{e.button.x, e.button.y};
                this->currentScene->handleButtonClickEnd(mousePoint);
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            {
                if (!this->currentScene)
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
                this->currentScene->handleKeyDown(e);
                this->currentScene->keyPressed(e.key.key);
                break;
            }
            case SDL_EVENT_TEXT_INPUT:
            {
                if (!this->currentScene)
                    break;
                this->currentScene->handleTextInput(e);
                break;
            }
            default:
                break;
            }
        }

        if (!this->currentScene)
        {
            continue;
        }

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

        if (this->currentScene)
        {
            this->currentScene->paint();
        }

        SDL_RenderPresent(this->render);

        const auto currentTime = static_cast<float>(SDL_GetTicks());
        deltaTime = (currentTime - static_cast<float>(lastFrameTime)) / 1000.0f;
        lastFrameTime = static_cast<Uint64>(currentTime);
        Physics::update(deltaTime);
        // Log("Body count ", physicsBodies.size());
        for (const PhysicsBody *body : currentScene->getPhysicsBodies())
        {
            // Log("Body ", bodies);
            const b2BodyId bodyId = body->getBody();
            if (Shape *shape = body->getUsedShape())
            {
                const auto [x, y] = b2Body_GetPosition(bodyId);
                const double angle = b2Rot_GetAngle(b2Body_GetRotation(bodyId));
                shape->setRotation(static_cast<float>(-(angle * (180 / M_PI))));

                const float pixelX = x * PIXELS_PER_METER;
                const float pixelY = -y * PIXELS_PER_METER;

                shape->setPosition(pixelX - shape->getW() / 2.0f, pixelY - shape->getH() / 2.0f);
                // Log("Setting shape position to ", x, ":",y);
                // Log("Setting shape angle to ", angle);
            }
        }
    }
    SDL_GetWindowSize(this->handle, &this->windowWidth, &this->windowHeight);
}

void Display::summonError() { Log("Init display first!"); }
} // namespace Proton
