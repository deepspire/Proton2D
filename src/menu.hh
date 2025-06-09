#pragma once
#include <SDL3/SDL_stdinc.h>
#include "proton/proton.hh"
#include "ss.hh"
#include "proton/shapes/rectangle.hh"
#include "proton/shapes/line.hh"
#include "proton/shapes/circle.hh"

class SecondScene;

class Menu : public Proton::Scene
{
public:
    Menu(SDL_Renderer *render, SDL_Window *window);

    void mouseDown(int x, int y) override;
    void keyPressed(Uint16 key) override;
    void deleteEvent() override;
    Proton::Scene* update(float dt) override;
};
