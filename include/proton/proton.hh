#pragma once

#ifndef __ANDROID__
#define SDL_MAIN_HANDLED
#endif

#include "scene.hh"
#include "shapes/shape.hh"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>

namespace Proton
{
class Display
{
  public:
    explicit Display(const std::string &title, int w = 480, int h = 640);
    void setScene(Scene *newScene);
    void startRendering();
    [[nodiscard]] auto getWindowHeight() const -> int;
    [[nodiscard]] auto getWindowWidth() const -> int;

    [[nodiscard]] auto getSurface() const -> SDL_Surface *;
    [[nodiscard]] auto getRenderer() const -> SDL_Renderer *;
    [[nodiscard]] auto getNativeWindow() const -> SDL_Window *;

    void setTitle(const char *title) const;
    void setIcon(const std::string &path) const;
    void setRenderScale(double x, double y) const;

    int pointerX, pointerY;

  private:
    void renderStart();
    static void summonError();

    SDL_Window *handle;
    SDL_Renderer *render;
    std::string title;
    std::vector<Shape> objects;
    Scene *currentScene;
    bool isInit = false;
    int windowWidth, windowHeight;
};
} // namespace Proton
