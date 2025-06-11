#pragma once
#define SDL_MAIN_HANDLED // без этого линковщик ломается на винде
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <thread>
#include <iostream>
#include "logman.hh"
#include "audio.hh"
#include "scene.hh"
#include "shapes/shape.hh"
#include "resourcemanager.hh"

namespace Proton
{
  class Display
  {
  public:
    Display(std::string title, int w = 480, int h = 640)
    {
      this->title = title;
      int ret = SDL_Init(SDL_INIT_VIDEO);
      TTF_Init();
      if (ret < 0)
      {
        Proton::Log("Error initializing SDL: ", SDL_GetError());
        return;
      }
      this->handle = SDL_CreateWindow(this->title.c_str(), w, h, 0);

      if (!this->handle)
      {
        Proton::Log("Error creating window: ", SDL_GetError());
        return;
      }

      this->randr = SDL_CreateRenderer(this->handle, NULL);
      if (!this->randr)
      {
        Proton::Log("Error initializing display: ", SDL_GetError());
        return;
      }

      SDL_SetRenderVSync(this->randr, 1);

      Proton::Log("display init is successful");
      this->isInit = true;
      this->currentScene = nullptr;
      Proton::initAudioEngine();
    }

    void setScene(Scene *newScene)
    {
      if (this->currentScene != nullptr)
      {
        currentScene->deleteEvent();
        delete currentScene;
        Proton::Log("Deleting previous scene...");
        // this->currentScene->clearScene(); // зачем чистить сцену если ее можно удалить
      }

      this->currentScene = newScene;
    }

    void startRendering()
    {
      if (this->isInit)
      {
        this->renderStart();
        delete this->currentScene;
        SDL_DestroyWindow(this->handle);
        SDL_DestroyRenderer(this->randr);
        ResourceManager::getInstance().clearCache();
        TTF_Quit();
        SDL_Quit();
        Proton::destroyAudioEngine();
      }
      else
      {
        summonError();
        return;
      }
    }

    SDL_Surface *getSurface() { return SDL_GetWindowSurface(this->handle); }

    SDL_Renderer *getRenderer() { return this->randr; }

    SDL_Window *getNativeWindow() { return this->handle; }

    void setTitle(const char* title)
    {
      SDL_SetWindowTitle(this->handle, title);
    }

    void setIcon(std::string path)
    {
      SDL_Surface* icon = ResourceManager::getInstance().getIcon(path);
      SDL_SetWindowIcon(this->handle, icon);
    }

    int pointerX, pointerY = 0;

  private:
    void renderStart()
    {
      SDL_Event e;
      bool isDone = false;

      /*const int TARGET_FPS = 120;
      const Uint64 FRAME_DELAY = 1000 / TARGET_FPS;*/

      Uint64 lastFrameTime = SDL_GetTicks();
      float deltaTime = 0.0f;
      while (!isDone)
      {
        // Uint64 frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&e))
        {
          switch (e.type)
          {
          case SDL_EVENT_QUIT:
            isDone = true;
            break;
          case SDL_EVENT_MOUSE_BUTTON_DOWN:
            this->currentScene->mouseDown(e.button.x, e.button.y);
            this->currentScene->handleButtonClick(e.button.x, e.button.y);
            break;
          case SDL_EVENT_KEY_DOWN:
            this->currentScene->keyPressed(e.key.key);
            this->currentScene->handleKeyDown(e);
            break;
          case SDL_EVENT_TEXT_INPUT:
            this->currentScene->handleTextInput(e);
            break;
          default:
            break;
          }
        }

        if (!this->currentScene)
        {
          isDone = true;
          continue;
        }
        else
        {
          SDL_SetRenderDrawColor(this->randr, this->currentScene->getBackgroundColor().getR(),
                                              this->currentScene->getBackgroundColor().getG(),
                                              this->currentScene->getBackgroundColor().getB(),
                                              this->currentScene->getBackgroundColor().getA());
        }

        float px, py;
        SDL_GetMouseState(&px, &py);
        pointerX = (int)px;
        pointerY = (int)py;
        SDL_RenderClear(this->randr);

        Scene *nextScene = this->currentScene->update(deltaTime);

        if (nextScene != this->currentScene)
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

        SDL_RenderPresent(this->randr);

        Uint64 currentTime = SDL_GetTicks();
        deltaTime = (currentTime - lastFrameTime) / 1000.0f;
        lastFrameTime = currentTime;
      }
    }

    void summonError() { Proton::Log("Init display first!"); }

    SDL_Window *handle;
    SDL_Renderer *randr;
    std::string title;
    std::vector<Shape> objects;
    Scene *currentScene;
    bool isInit = false;
  };

}
