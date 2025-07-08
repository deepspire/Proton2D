#pragma once

#include <SDL3/SDL_stdinc.h>
#include "proton/scene.hh"
#include "menu.hh"

class Menu;

class SecondScene : public Proton::Scene
{
public:
    SecondScene(SDL_Renderer *render, SDL_Window *window);

    void mouseDown(const Proton::Point &mPos) override;
    void keyPressed(Uint16 key) override;

    ~SecondScene() override;

    Proton::Scene* update(float dt) override;
};
