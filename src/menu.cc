#include "menu.hh"
#include "ss.hh"
#include "proton/shapes/rectangle.hh"
#include "proton/shapes/line.hh"
#include "proton/shapes/circle.hh"
#include <SDL3/SDL_stdinc.h>

Menu::Menu(Proton::Display *display)
{
    this->display = display;
    this->render = display->getRenderer();
    display->setBackground(Proton::Color(255, 255, 255));
}

void Menu::mouseDown()
{
    display->setScene(new SecondScene(display));
}

void Menu::keyPressed(Uint16 ev) {}
void Menu::update(Uint64 dt)
{
    clearScene();
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            Proton::Rectangle *rect = new Proton::Rectangle(i * 2, j * 2, 2, 2, Proton::Color(rand() % 256, rand() % 256, rand() % 256));
            addObject(rect);
        }
    }
    addObject(new Proton::Rectangle(480 / 2 - 25, 640 / 2 - 25, 50, 50, Proton::Color()));
    addObject(new Proton::Line(480 / 2, 640 / 2, 480, 640, Proton::Color()));
    addObject(new Proton::Line(0, 640, 480 / 2, 640 / 2, Proton::Color()));
    addObject(new Proton::Circle(480 / 2, 640 / 2 + 100, 50, Proton::Color(0, 255, 0)));
}
