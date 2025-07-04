#include "menu.hh"

Proton::Rectangle *moving;
bool movingflag = false;

Menu::Menu(SDL_Renderer *render, SDL_Window *window) : Scene(render, window)
{
    // addObject(new Proton::Rectangle(480, 640, 50, 50, Proton::Color(), 20));
    moving = new Proton::Rectangle(200, 600, 25, 50, Proton::Color());
    addObject(moving);
    for (int i = 0; i <= 200; i += 1)
    {
        addObject(new Proton::Rectangle(i, 640, 25, 50, Proton::Color()));
    }
    addObject(new Proton::Line(480 / 2, 640 / 2, 480, 640, Proton::Color()));
    addObject(new Proton::Line(0, 640, 480 / 2, 640 / 2, Proton::Color()));

    addObject(new Proton::Line(0, 640, 480 / 1, 640 / 1, Proton::Color()));
    addObject(new Proton::Line(480 / 1, 640 / 1, 480, 640, Proton::Color()));
    addObject(new Proton::Circle(480 / 2, 640 / 2 + 100, 50, Proton::Color(255, 0, 255), Proton::Circle::Fill));
    addObject(new Proton::Circle(480 / 2, 640 / 2 + 100, 50, Proton::Color(0, 255, 0), Proton::Circle::Bevel));
}

void Menu::mouseDown(int x, int y)
{
    this->goNextScene = true;
    this->nextScene = new SecondScene(this->render, this->window);
}

void Menu::keyPressed([[maybe_unused]] Uint16 ev) {}
Proton::Scene *Menu::update(float dt)
{
    if (movingflag)
    {
        moving->setPosition(moving->getX() - 1, moving->getY());
        if (moving->getX() <= 0)
        {
            moving->setPosition(600, moving->getY());
            movingflag = false;
        }
    }
    else
    {
        moving->setPosition(moving->getX() + 1, moving->getY());
        if (moving->getX() >= 600)
        {
            movingflag = true;
        }
    }

    if (this->goNextScene)
    {
        return this->nextScene;
    }

    return this;
}