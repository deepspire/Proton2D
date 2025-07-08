#pragma once
#include <SDL3/SDL_stdinc.h>
#include "proton/scene.hh"

class SecondScene;

class Menu : public Proton::Scene
{
public:
    Menu(SDL_Renderer *render, SDL_Window *window);

    void mouseDown(const Proton::Point &mPos) override;
    void keyPressed(Uint16 key) override;
    Proton::Scene* update(float dt) override;
};
