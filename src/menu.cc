#include "menu.hh"
#include "proton/shapes/circle.hh"
#include "proton/shapes/rectangle.hh"
#include "ss.hh"

Proton::Rectangle *moving;
Proton::Circle *r1;
bool movingflag = false;

Menu::Menu(SDL_Renderer *render, SDL_Window *window) : Scene(render, window)
{
    // addObject(new Proton::Rectangle(480, 640, 50, 50, Proton::Color(), 20));
    moving = new Proton::Rectangle(200, 600, 50, 50, Proton::Color());
    moving->setRotation(60);
    addObject(moving);
    /*for (int i = 0; i <= 200; i += 1)
    {
        addObject(new Proton::Rectangle(i, 640, 25, 50, Proton::Color()));
    }*/
    r1 = new Proton::Circle(240, 320 + 100, 50, Proton::Color(255, 0, 255), Proton::Circle::Fill);
    addObject(r1);
    addObject(new Proton::Circle(240, 320 + 100, 50, Proton::Color(0, 255, 0), Proton::Circle::Bevel));
}

void Menu::mouseDown(int x, int y)
{
    this->goNextScene = true;
    this->nextScene = new SecondScene(this->render, this->window);
}

void Menu::keyPressed([[maybe_unused]] Uint16 ev) {}
Proton::Scene *Menu::update(const float dt)
{
    moving->setRotation(moving->getRotation() + dt * 40);
    r1->setRotation(r1->getRotation() + dt * 100);
    if (movingflag)
    {
        moving->setPosition(moving->getX() - 40 * dt, moving->getY());
        if (moving->getX() <= 0)
        {
            moving->setPosition(600, moving->getY());
            movingflag = false;
        }
    }
    else
    {
        moving->setPosition(moving->getX() + 40 * dt, moving->getY());
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