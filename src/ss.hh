#pragma once
#include <SDL3/SDL_stdinc.h>
#include "proton/proton.hh"
#include "menu.hh"
#include "proton/shapes/rectangle.hh"
#include "proton/shapes/line.hh"
#include "proton/shapes/circle.hh"
#include "proton/shapes/text.hh"
#include "proton/shapes/image.hh"
#include "proton/shapes/rectanglebutton.hh"
#include "proton/shapes/imagebutton.hh"
#include "proton/shapes/container.hh"
#include "proton/shapes/group.hh"

class Menu;

class SecondScene : public Proton::Scene
{
public:
    SecondScene(SDL_Renderer *render, SDL_Window *window);

    void mouseDown(int x, int y) override;
    void keyPressed(Uint16 key) override;
    Proton::Scene* update(float dt) override;
};
