#pragma once
#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>

#include "logman.hh"
#include "scene.hh"
#include "shapes/shape.hh"
#include "resourcemanager.hh"

namespace Proton
{
  class Display
  {
  public:
    explicit Display(const std::string &title, int w = 480, int h = 640);
    void setScene(Scene *newScene);
    void startRendering();

    [[nodiscard]] SDL_Surface *getSurface() const;
    [[nodiscard]] SDL_Renderer *getRenderer() const;
    [[nodiscard]] SDL_Window *getNativeWindow() const;

    void setTitle(const char *title) const;
    void setIcon(const std::string &path) const;

    int pointerX, pointerY = 0;

  private:
    void renderStart();
    static void summonError();

    SDL_Window *handle;
    SDL_Renderer *render;
    std::string title;
    std::vector<Shape> objects;
    Scene *currentScene;
    bool isInit = false;
  };
}
