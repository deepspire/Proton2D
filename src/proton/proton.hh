#pragma once
#define SDL_MAIN_HANDLED // без этого линковщик ломается на винде
#include "scene.hh"
#include "shapes/shape.hh"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <thread>
#include <iostream>
#include "logman.hh"

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
    }

    void setScene(Scene *newScene)
    {
      if (this->currentScene != nullptr)
      {
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
        TTF_Quit();
        SDL_Quit();
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

    void setBackground(Color color) { this->background = color; }

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
            this->currentScene->mouseDown();
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
        float px, py;
        SDL_GetMouseState(&px, &py);
        pointerX = (int)px;
        pointerY = (int)py;
        SDL_SetRenderDrawColor(this->randr,
                               background.getR(), background.getG(),
                               background.getB(), background.getA());
        SDL_RenderClear(this->randr);

        this->currentScene->update(deltaTime); // сначало обновляем объекты а потом уже их рисуем
        this->currentScene->paint(deltaTime);

        SDL_RenderPresent(this->randr);

        /*Uint64 frameTime = SDL_GetTicks() - frameStart;

        if (frameTime < FRAME_DELAY)
        {
          SDL_Delay(FRAME_DELAY - frameTime);
        }*/

        Uint64 currentTime = SDL_GetTicks();
        deltaTime = (currentTime - lastFrameTime) / 1000.0f;
        lastFrameTime = currentTime;
      }
    }

    void summonError() { Proton::Log("Init display first!"); }

    SDL_Window *handle;
    SDL_Renderer *randr;
    std::string title;
    Color background;
    std::vector<Shape> objects;
    Scene *currentScene;
    bool isInit = false;
  };

}
