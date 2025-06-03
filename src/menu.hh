#pragma once
#include "proton/proton.hh"
#include <SDL3/SDL_stdinc.h>

class SecondScene;

class Menu : public Proton::Scene
{
public:
    Menu(Proton::Display *display);

    void mouseDown() override;
    void keyPressed(Uint16 key) override;
    void update(Uint64 dt) override;

private:
    Proton::Display *display;
};
