#pragma once
#include "proton/proton.hh"

class Menu;

class SecondScene : public Proton::Scene
{
public:
    SecondScene(Proton::Display *display);

    void mouseDown() override;
    void keyPressed(Uint16 key) override;
    void update(Uint64 dt) override;

private:
    Proton::Display *display;
};
